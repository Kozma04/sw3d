#include "pipeline.h"


static void swap(int32_t &a, int32_t &b) {
    int32_t c = a;
    a = b;
    b = c;
}

static int32_t *g_varying_buf_vert[4]; // Position 0 is reserved for the interpolated attributes.
static void *g_uniforms;
static pipeline_processor_t g_proc;

static int32_t g_width, g_height;
static uint8_t* g_bmp;
static uint16_t* g_depthbuf;

static uint32_t g_features;
static uint16_t g_depth_min, g_depth_max;

struct vec2i_t {
    int32_t x, y;
};


void tgfx_init_surface(int32_t width, int32_t height, uint8_t* grb_buf, uint16_t* depth_buf) {
    g_width = width;
    g_height = height;
    g_bmp = grb_buf;
    g_depthbuf = depth_buf;
}

void tgfx_enable(tgfx_feature_t feature) {
    g_features |= feature;
}

void tgfx_disable(tgfx_feature_t feature) {
    g_features &= ~feature;
}

void tgfx_depth_clip(uint16_t clip_min, uint16_t clip_max) {
    g_depth_min = clip_min;
    g_depth_max = clip_max;
}

void tgfx_proc_load(const pipeline_processor_t *proc) {
    uint32_t varying_size = 0;
    uint32_t uniform_size = 0;
    for(int i = 0; i < proc->n_varying; i++)
        varying_size += proc->varying_fmt[i].count * sizeof(int32_t);
    for(int i = 0; i < sizeof(g_varying_buf_vert) / sizeof(g_varying_buf_vert[0]); i++)
        g_varying_buf_vert[i] = (int32_t*)malloc(varying_size);
    g_proc = *proc;
}

void tgfx_proc_unload() {
    for(int i = 0; i < sizeof(g_varying_buf_vert) / sizeof(g_varying_buf_vert[0]); i++) {
        free(g_varying_buf_vert[i]);
        g_varying_buf_vert[i] = nullptr;
    }
}

void tgfx_set_proc_uniform_buf(void* buff) {
    g_uniforms = buff;
}

void tgfx_draw_tris(void* const vertex_buf, uint16_t n_tris) {
    uint8_t* bp = (uint8_t*)vertex_buf;
    for (uint16_t i = 0; i < n_tris; i++) {
        tgfx_draw_tri(bp);
        bp += 3 * g_proc.vertex_size;
    }
}

void tgfx_draw_tris_idx(void* vertex_buf, uint16_t* index_buf, uint16_t n_tris) {
    uint8_t* vert_buf_u8 = (uint8_t*)vertex_buf;
    uint8_t* tri_buf = (uint8_t*)_malloca(3 * g_proc.vertex_size);
    uint16_t vert_size = g_proc.vertex_size;
    for(uint16_t i = 0; i < n_tris * 3;) {
        memcpy(tri_buf, vert_buf_u8 + (uint32_t)index_buf[i++] * vert_size, vert_size);
        memcpy(tri_buf + vert_size, vert_buf_u8 + (uint32_t)index_buf[i++] * vert_size, vert_size);
        memcpy(tri_buf + vert_size * 2, vert_buf_u8 + (uint32_t)index_buf[i++] * vert_size, vert_size);
        tgfx_draw_tri(tri_buf);
    }
}


struct tgfx_scanline_t {
    // Interpolation
    int32_t w0, w1;
    int32_t d_w0_dx, d_w1_dx;
    // Coordinates
    int32_t start_x, end_x;
    int32_t y;
    // Color and depth buffers
    uint8_t* p_bmp_line;
    uint16_t* p_depth_line;
    // Vertices' depth
    int32_t z_inv[3], z[3];
};

static void scanline(tgfx_scanline_t params) {
    int32_t w2;
    int64_t depth_inv, depth = 1 << 16;
    uint32_t buf_idx;
    int64_t interp;
    for (; params.start_x <= params.end_x; params.start_x++, params.w0 += params.d_w0_dx, params.w1 += params.d_w1_dx, params.p_depth_line++, params.p_bmp_line += 3) {
        w2 = 65535 - params.w0 - params.w1;

        if (g_features & TGFX_DEPTH_CALC) {
            depth = ((int64_t)params.w0 * params.z[0]);
            depth += ((int64_t)params.w1 * params.z[1]);
            depth += ((int64_t)w2 * params.z[2]);
            depth >>= 16;
        }
        
        if (g_features & TGFX_PERSPECTIVE_DIVIDE) {
            depth_inv = ((int64_t)params.w0 * params.z_inv[0]);
            depth_inv += ((int64_t)params.w1 * params.z_inv[1]);
            depth_inv += ((int64_t)w2 * params.z_inv[2]);
            depth_inv >>= 16;
            if(depth_inv != 0)
                depth_inv = (0x7fffffff / depth_inv) << 1;
            depth_inv &= 0x7fffffff;
            depth = ((int64_t)depth * depth_inv) >> 16;
        }

        int32_t depth16 = depth;// >> 16;

        if (g_features & TGFX_DEPTH_CLIP) {
            if(depth16 < g_depth_min || depth16 > g_depth_max)
                continue;
        }

        if((g_features & TGFX_DEPTH_TEST_LOWER) && depth16 >= *params.p_depth_line) continue;
        if((g_features & TGFX_DEPTH_TEST_HIGHER) && depth16 <= *params.p_depth_line) continue;

        if(g_features & TGFX_DEPTH_WRITE) *params.p_depth_line = depth16;
        

        buf_idx = 0;
        for(int32_t i = 0; i < g_proc.n_varying; i++) {
            if(g_proc.varying_fmt[i].interpolate) {
                for(int32_t j = 0; j < g_proc.varying_fmt[i].count; j++) {
                    interp = (((int64_t)params.w0 * g_varying_buf_vert[1][buf_idx]));
                    interp += (((int64_t)params.w1 * g_varying_buf_vert[2][buf_idx]));
                    interp += (((int64_t)w2 * g_varying_buf_vert[3][buf_idx]));
                    interp >>= 16;
                    if(g_features & TGFX_PERSPECTIVE_DIVIDE)
                        interp = ((int64_t)interp * depth_inv) >> 16;
                    g_varying_buf_vert[0][buf_idx] = interp;
                    buf_idx++;
                }
            }
            else
                for(int j = 0; j < g_proc.varying_fmt[i].count; j++)
                    g_varying_buf_vert[0][buf_idx] = g_varying_buf_vert[1][buf_idx];
        }

        g_proc.process_pixel(
            { params.start_x, params.y, (int32_t)depth, g_varying_buf_vert[0], g_uniforms },
            { params.p_bmp_line }
        );
    }
}

void tgfx_draw_tri(void* const buff) {
    int32_t x0, y0, x1, y1, x2, y2;
    int32_t z0, z1, z2;
    int32_t z0_inv, z1_inv, z2_inv;
    g_proc.process_vertex(
            {buff, g_uniforms},
            {&x0, &y0, &z0, &z0_inv, g_varying_buf_vert[1]}
    );
    g_proc.process_vertex(
            {(uint8_t*)(buff) + g_proc.vertex_size, g_uniforms},
            {&x1, &y1, &z1, &z1_inv, g_varying_buf_vert[2]}
    );
    g_proc.process_vertex(
            {(uint8_t*)(buff) + g_proc.vertex_size * 2, g_uniforms},
            {&x2, &y2, &z2, &z2_inv, g_varying_buf_vert[3]}
    );

    if(y0 == y1 && y0 == y2) return;
    if (z0 == 0 || z1 == 0 || z2 == 0) return;
    if (g_features & TGFX_DEPTH_CLIP) {
        if ((z0 < g_depth_min || z0 > g_depth_max) && (z1 < g_depth_min || z1 > g_depth_max) && (z2 < g_depth_min || z2 > g_depth_max))
            return;
    }

    if(g_features & TGFX_PERSPECTIVE_DIVIDE) {
        uint32_t buf_idx = 0;
        for(uint32_t i = 0; i < g_proc.n_varying; i++) {
            for(uint32_t j = 0; j < g_proc.varying_fmt[i].count; j++, buf_idx++) {
                g_varying_buf_vert[1][buf_idx] = ((int64_t)g_varying_buf_vert[1][buf_idx] * z0_inv) >> 16;
                g_varying_buf_vert[2][buf_idx] = ((int64_t)g_varying_buf_vert[2][buf_idx] * z1_inv) >> 16;
                g_varying_buf_vert[3][buf_idx] = ((int64_t)g_varying_buf_vert[3][buf_idx] * z2_inv) >> 16;
            }
        }
    }
    
    int32_t orig_x0 = x0, orig_y0 = y0;
    int32_t orig_x1 = x1, orig_y1 = y1;
    int32_t orig_x2 = x2, orig_y2 = y2;

    if(y0 > y2) {
        swap(x0, x2);
        swap(y0, y2);
    }
    if(y0 > y1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    if(y1 > y2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    int64_t area_abc_fixed = (((orig_x2 - orig_x0) * (orig_y1 - orig_y0) - (orig_y2 - orig_y0) * (orig_x1 - orig_x0))); // in pixels, 1 integer = 65536
    if(area_abc_fixed == 0) return;
    if(g_features & TGFX_CULL_FACE) {
        if ((g_features & TGFX_CULL_FACE_CW) && area_abc_fixed > 0) return;
        else if (!(g_features & TGFX_CULL_FACE_CW) && area_abc_fixed < 0) return;
    }
    int64_t area_abc_inv_fp = ((int64_t)1 << 32) / area_abc_fixed;// ; // f15p16

    int32_t y_mid = y1;
    int32_t x_mid = 0;
    if(y2 == y0) x_mid = x2;
    else x_mid = x0 + (x2 - x0) * (y_mid - y0) / (y2 - y0);
    uint8_t color[3];


    int32_t clip_y_start = y0;
    int32_t clip_y_end = y_mid;

    uint8_t* p_bmp_line;
    uint16_t* p_depth_line;
    int32_t dx_start, dx_end, fp_start_x, fp_end_x;

    int32_t d_w0_dx = orig_y2 - orig_y1;
    int32_t d_w0_dy = orig_x1 - orig_x2;
    int32_t w0_const = orig_x1 * (orig_y1 - orig_y2) + orig_y1 * (orig_x2 - orig_x1);

    int32_t d_w1_dx = orig_y0 - orig_y2;
    int32_t d_w1_dy = orig_x2 - orig_x0;
    int32_t w1_const = orig_x2 * (orig_y2 - orig_y0) + orig_y2 * (orig_x0 - orig_x2);

    if(abs(area_abc_inv_fp) < 10000) return;

    d_w0_dx = ((int64_t)d_w0_dx * area_abc_inv_fp) >> 16;
    d_w0_dy = ((int64_t)d_w0_dy * area_abc_inv_fp) >> 16;
    w0_const = ((int64_t)w0_const * area_abc_inv_fp) >> 16;
    d_w1_dx = ((int64_t)d_w1_dx * area_abc_inv_fp) >> 16;
    d_w1_dy = ((int64_t)d_w1_dy * area_abc_inv_fp) >> 16;
    w1_const = ((int64_t)w1_const * area_abc_inv_fp) >> 16;

    int64_t w0_line, w1_line;

    tgfx_scanline_t scan;
    scan.d_w0_dx = d_w0_dx;
    scan.d_w1_dx = d_w1_dx;
    scan.z[0] = z0, scan.z[1] = z1, scan.z[2] = z2;
    scan.z_inv[0] = z0_inv, scan.z_inv[1] = z1_inv, scan.z_inv[2] = z2_inv;

    // Draw flat bottom triangle
    if(y1 != y0) {
        if (clip_y_start < 0) clip_y_start = 0;
        if (clip_y_end >= g_height) clip_y_end = g_height - 1;

        dx_start = ((x1 - x0) << 16) / (y1 - y0);
        dx_end = ((x_mid - x0) << 16) / (y_mid - y0);

        if (dx_start > dx_end) swap(dx_start, dx_end);
        if (x0 < 0 && dx_end != 0) {
            clip_y_start += ((-x0) << 16) / dx_end + 1;
        }

        fp_start_x = (x0 << 16) + dx_start * (clip_y_start - y0);
        fp_end_x = (x0 << 16) + dx_end * (clip_y_start - y0);
        p_bmp_line = g_bmp + 3 * (clip_y_start * g_width);
        p_depth_line = g_depthbuf + clip_y_start * g_width;


        w0_line = w0_const + clip_y_start * d_w0_dy;
        w1_line = w1_const + clip_y_start * d_w1_dy;
        int32_t y = clip_y_start;
        while(y <= clip_y_end) {
            int32_t start_x = (fp_start_x >> 16);
            int32_t end_x = (fp_end_x >> 16);

            if(start_x < 0) start_x = 0;
            if(end_x >= g_width) end_x = g_width - 1;

            scan.w0 = w0_line + start_x * d_w0_dx;
            scan.w1 = w1_line + start_x * d_w1_dx;
            scan.start_x = start_x;
            scan.end_x = end_x;
            scan.y = y;
            scan.p_bmp_line = p_bmp_line + 3 * start_x;
            scan.p_depth_line = p_depth_line + start_x;

            scanline(scan);

            y++;
            fp_start_x += dx_start;
            fp_end_x += dx_end;
            w0_line += d_w0_dy;
            w1_line += d_w1_dy;
            p_bmp_line += 3 * g_width;
            p_depth_line += g_width;
        }
    }

    // Draw flat top triangle
    if(y2 != y_mid) {
        clip_y_start = y_mid + 1;
        if (clip_y_start < 0) clip_y_start = 0;
        clip_y_end = y2 >= g_height ? g_height - 1 : y2;
        uint16_t diff_y_inv = (1 << 16) / (y2 - y_mid);

        p_bmp_line = g_bmp + 3 * (clip_y_start * g_width);
        p_depth_line = g_depthbuf + clip_y_start * g_width;
        dx_start = (x2 - x1) * diff_y_inv;
        dx_end = (x2 - x_mid) * diff_y_inv;
        fp_start_x = (x1 << 16) + (clip_y_start - y_mid) * dx_start;
        fp_end_x = (x_mid << 16) + (clip_y_start - y_mid) * dx_end;
        if (fp_start_x > fp_end_x) {
            swap(fp_start_x, fp_end_x);
            swap(dx_start, dx_end);
        }

        w0_line = w0_const + clip_y_start * d_w0_dy;
        w1_line = w1_const + clip_y_start * d_w1_dy;
        int32_t y = clip_y_start;

        while(y <= clip_y_end) {
            int32_t start_x = (fp_start_x >> 16);
            int32_t end_x = (fp_end_x >> 16);

            if(start_x < 0) start_x = 0;
            if(end_x >= g_width) end_x = g_width - 1;

            scan.w0 = w0_line + start_x * d_w0_dx;
            scan.w1 = w1_line + start_x * d_w1_dx;
            scan.start_x = start_x;
            scan.end_x = end_x;
            scan.y = y;
            scan.p_bmp_line = p_bmp_line + 3 * start_x;
            scan.p_depth_line = p_depth_line + start_x;

            scanline(scan);

            y++;
            fp_start_x += dx_start;
            fp_end_x += dx_end;
            w0_line += d_w0_dy;
            w1_line += d_w1_dy;
            p_bmp_line += 3 * g_width;
            p_depth_line += g_width;
        }
    }
}
