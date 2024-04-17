#include "main.h"
#include <cstdlib>
#include <cmath>
#include "3dmath.h"
#include "pipeline.h"
#include "shaders.h"
#define M_PI 3.14159


pipeline_processor_t proc_basic3d = gen_processor_basic_3d();

void update_fpscnt() {
    static uint32_t last_sample = time_ms();
    static uint32_t fpscnt = 0;
    if(time_ms() < last_sample + 1000)
        fpscnt++;
    else {
        printf("FPS: %u; avg. frametime: %fms\n", fpscnt, (float)(time_ms() - last_sample) / fpscnt);
        fpscnt = 0;
        last_sample = time_ms();
    }
}

bool keyDown(char key) {
    return GetKeyState(key) & 0x8000;
}

texture_t tex_mesh, tex_hmap;
basic3d_uniforms_t unif;
uint8_t bmp_file[1024 * 1024];

int32_t hmap_vert[65536];
uint16_t hmap_ind[65536];
uint16_t hmap_tris;
uint16_t *g_depth;

float rot_pitch = 0.27, rot_yaw = 3.86;
float cam_x = 6.2, cam_y = -33.4, cam_z = 5.8;
float delta_t = 1;
float cam_speed = 5;

void render() {
    int32_t mx, my;
    window_mousepos_delta(&mx, &my);
    rot_yaw += mx * 0.002;
    rot_pitch += my * 0.002;
    
    float cam_dx = 0, cam_dz = 0;
    if (keyDown('A')) cam_dx -= cam_speed * delta_t;
    if (keyDown('D')) cam_dx += cam_speed * delta_t;
    if (keyDown('W')) cam_dz += cam_speed * delta_t;
    if (keyDown('S')) cam_dz -= cam_speed * delta_t;
    if (keyDown('Q')) cam_y -= cam_speed * delta_t;
    if (keyDown('E')) cam_y += cam_speed * delta_t;
    cam_z += cam_dz * cos(rot_yaw) + cam_dx * -sin(rot_yaw);
    cam_x += cam_dz * sin(rot_yaw) + cam_dx * cos(rot_yaw);

    /*window_mousepos(&mx, &my);
    mx /= 4, my /= 4;
    int32_t d = g_depth[my * g_width + mx];
    printf("DEPTH %u x %u: %d.%d\n", mx, my, d >> 16, d & 0xffff);*/


    uint32_t frame_start = time_ms();
    //window_clear_draw_buffer();
    for (int i = 0; i < g_width * g_height; i++) {
        uint8_t* p = g_bmp + i * 3;
        p[0] = 116;
        p[1] = 99;
        p[2] = 108;
    }
    for (int i = 0; i < g_width * g_height; i++)
        g_depth[i] = 65535;

    float x0 = rand() % 800, y0 = rand() % 600;
    float x1 = rand() % 800, y1 = rand() % 600;
    float x2 = rand() % 800, y2 = rand() % 600;

    for(int i = 0; i < 1; i++) {

        float t = time_ms() / 1000.f / 2 + i / 10.f;
        float a0 = t, a1 = t + M_PI * 2 / 3, a2 = t + M_PI * 4 / 3;
        x0 = cos(a0) * 256, y0 = sin(a0) * 256;
        x1 = cos(a1) * 256, y1 = sin(a1) * 256;
        x2 = cos(a2) * 256, y2 = sin(a2) * 256;

        float vert_buf[] = {
                x0, y0, 0,  0, 0, 0,
                x1, y1, 0,  0, 0, 0,
                x2, y2, 0, 0, 0, 1
        };

        int k = 1;
        int32_t vert_buf_f16p16[] = {
            0,     0,     65535 * 0,   0xff, 0, 0,  0, 0,
            0,     65536, 65535 * 0,   0, 0xff, 0,  0xffff, 0,
            65536, 0,     65535 * 0,   0, 0, 0xff,  0, 0xffff,
            65536, 65536, 65535 * 0,   0xff, 0xff,  0xffff, 0xffff, 0xffff
        };
        uint16_t ind_buf[] = {
            0, 1, 2,
            1, 2, 3
        };

        mat4_16p16_t temp, temp2, temp3, temp4;
        transform_translatef(&temp4, cam_x, cam_y, cam_z);
        transform_rotatef(&temp2, 0, -rot_yaw, 0);
        transform_rotatef(&temp3, -rot_pitch, 0, 0);
        mul_mat4_16p16(&temp3, &temp2, &temp);
        mul_mat4_16p16(&temp, &temp4, &temp2);

        //transform_perspectivef(&temp, (float)g_height / g_width, 120 / 180.f * 3.141592, 16, 32);
        transform_perspective(&temp, 36700, 90 / 180.f * 3.141592 * 65536, 0.1 * 65536, 64 * 65536);
        //tra_mat4_16p16(&temp, &temp4);
        mul_mat4_16p16(&temp, &temp2, &unif.mvp);
        //unif.mvp = temp2;


        //gfx_draw_tris_idx(vert_buf_f16p16, ind_buf, 2);
        tgfx_draw_tris_idx(hmap_vert, hmap_ind, hmap_tris);
    }
    delta_t = (time_ms() - frame_start) * .001f;

    //Sleep(20);

    //window_swap_buffers();
    update_fpscnt();
}

void put_vert(int32_t* p, int32_t x, int32_t y, int32_t z, int32_t u, int32_t v) {
    p[0] = x, p[1] = y, p[2] = z;
    p[3] = 0, p[4] = 0, p[5] = 0;
    p[6] = u, p[7] = v;
}

int32_t hmap_val(texture_t* tex, uint32_t x, uint32_t y) {
    uint8_t rgb[3];
    tgfx_rgb888(tex, x, y, rgb);
    return ((int32_t)(rgb[0]) << 16) / 256.f * 128;
}

void gen_heightmap(texture_t *hmap, int32_t* vert, uint16_t* ind, uint16_t *tris, int32_t scale_xz, int32_t scale_y, uint16_t xstart, uint16_t ystart, uint16_t width, uint16_t height) {
    uint32_t vp = 0, ip = 0;
    uint32_t v_size = 8;
    *tris = 0;
    int32_t unit = scale_xz;
    int32_t alpha = 0;
    for(uint32_t x = 0; x < width - 1; x++) {
        for(uint32_t y = 0; y < height - 1; y++) {
            int32_t tl = ((int64_t)hmap_val(hmap, x + xstart, y + ystart) * scale_y) >> 16;
            int32_t tr = ((int64_t)hmap_val(hmap, x + xstart + 1, y + ystart) * scale_y) >> 16;
            int32_t bl = ((int64_t)hmap_val(hmap, x + xstart, y + ystart + 1) * scale_y) >> 16;
            int32_t br = ((int64_t)hmap_val(hmap, x + xstart + 1, y + ystart + 1) * scale_y) >> 16;

            put_vert(vert + vp * v_size, x * unit - alpha, tl, y * unit + alpha, (x << 16) / width, (y << 16) / height);
            put_vert(vert + (vp + 1) * v_size, (x + 1) * unit + alpha, tr, y * unit - alpha, ((x + 1) << 16) / width, (y << 16) / height);
            put_vert(vert + (vp + 2) * v_size, x * unit - alpha, bl, (y + 1) * unit + alpha, (x << 16) / width, ((y + 1) << 16) / height);
            put_vert(vert + (vp + 3) * v_size, (x + 1) * unit + alpha, br, (y + 1) * unit + alpha, ((x + 1) << 16) / width, ((y + 1) << 16) / height);
            ind[ip] = vp;
            ind[ip + 1] = vp + 1;
            ind[ip + 2] = vp + 2;
            ind[ip + 3] = vp + 3;
            ind[ip + 4] = vp + 2;
            ind[ip + 5] = vp + 1;
            
            vp += 4;
            ip += 6;
            *tris += 2;
        }
    }
}


tgfx_bmp_res_t load_texture_file(const char* path, texture_t* out) {
    FILE* file = fopen(path, "rb");
    uint32_t size;
    printf("Loading texture from %s: ", path);
    if (!file) {
        printf("Can't open texture file.\n");
        return TGFX_BMP_UNKNOWN_ERR;
    }
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("size %u\n", size);

    uint8_t* bmp = (uint8_t*)malloc(size);
    if (!bmp) {
        printf("Can't allocate %u bytes for texture.\n", size);
        return TGFX_BMP_UNKNOWN_ERR;
    }

    uint32_t n_read = fread(bmp, 1, size, file);
    if (n_read != size) {
        printf("Texture load failed, only read %u/%u bytes\n", n_read, size);
        return TGFX_BMP_UNKNOWN_ERR;
    }

    tgfx_bmp_res_t bmpres = tgfx_load_bitmap(bmp, out);

    if (bmpres != TGFX_BMP_OK) {
        printf("Texture bitmap load failed: %u\n", bmpres);
    }
    else printf("Texture loaded, format %u, %ux%u, %u bytes\n", out->fmt, out->width, out->height, size);
    return bmpres;
}

uint8_t setup() {
    util_init();
    g_depth = (uint16_t*)malloc(g_width * g_height * sizeof(uint16_t));
    tgfx_init_surface(g_width, g_height, g_bmp, g_depth);
    tgfx_proc_load(&proc_basic3d);

    TGFX_ASSERT(load_texture_file("D:\\sw3d_res\\bg1.bmp", &tex_mesh));
    TGFX_ASSERT(load_texture_file("D:\\sw3d_res\\heightmap_256.bmp", &tex_hmap));

    gen_heightmap(&tex_hmap, hmap_vert, hmap_ind, &hmap_tris, 65536 * 2, 65536 * 1, 0, 0, 32, 32);
    printf("hmap_tris=%u\n", hmap_tris);
    
    unif.uv_right_shift = 8;
    unif.tex = &tex_mesh;

    tgfx_set_proc_uniform_buf(&unif);
    tgfx_enable(TGFX_CULL_FACE);
    tgfx_disable(TGFX_CULL_FACE_CW);
    tgfx_enable(TGFX_PERSPECTIVE_DIVIDE);
    tgfx_enable(TGFX_DEPTH_WRITE);
    tgfx_enable(TGFX_DEPTH_CALC);
    tgfx_enable(TGFX_DEPTH_TEST_LOWER);
    tgfx_enable(TGFX_DEPTH_CLIP);
    tgfx_depth_clip(128, 65535);
    return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
    AllocConsole();
    freopen("conin$", "r", stdin);
    freopen("conout$", "w", stdout);
    freopen("conout$", "w", stderr);
    printf("Program Start\n");
    window_create(hInstance, 480, 272, 4);
    if (!setup()) {
        Sleep(100000);
        return 1;
    }
    window_loop(render);
    tgfx_proc_unload();


    return 0;
}
