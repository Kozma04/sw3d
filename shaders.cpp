#include "shaders.h"
#include "3dmath.h"


struct basic3d_vertex_attr {
    int32_t vx;
    int32_t vy;
    int32_t vz;
    int32_t r;
    int32_t g;
    int32_t b;
    int32_t u;
    int32_t v;
};

struct basic3d_varying_attr {
    int32_t r;
    int32_t g;
    int32_t b;
    int32_t u;
    int32_t v;
};


static pipeline_attr_t basic_3d_vert_attr[] = {
        {.count = 3, .type = PIPELINE_ATTR_TYPE_FIX_S15P16}, // position
        {.count = 3, .type = PIPELINE_ATTR_TYPE_FIX_S15P16}, // color
        {.count = 2, .type = PIPELINE_ATTR_TYPE_FIX_S15P16}, // texture coords.
};

static pipeline_varying_attr_t basic_3d_varying_attr[] = {
        {.count = 3, .interpolate = 1}, // colors
        {.count = 2, .interpolate = 1} // tex. coords
};


void basic_3d_process_vertex(process_vertex_input_t in, process_vertex_output_t out) {
    basic3d_uniforms_t* uniforms = (basic3d_uniforms_t*)in.user_uniform;
    basic3d_vertex_attr* vert = (basic3d_vertex_attr*)in.user_attr;
    basic3d_varying_attr* vary = (basic3d_varying_attr*)out.varying_out;

    vec4_16p16_t v_orig(vert->vx, vert->vy, vert->vz, 65536);
    vec4_16p16_t v;
    mul_mat4_16p16_vec4_16p16(&uniforms->mvp, &v_orig, &v);
    int32_t vx = v.v[0], vy = v.v[1], vz = v.v[2], vw = v.v[3];

    if (vz == 0 || vw == 0) {
        *out.depth = 0x7fffffff;
        return;
    }

    int32_t vw_inv = (0x7fffffff / vw) << 1;

    vx = ((int64_t)vx * vw_inv) >> 16;
    vy = ((int64_t)vy * vw_inv) >> 16;
    vx = (int64_t)(32768 + (vx >> 1)) * g_width;
    vy = (int64_t)(32768 + (vy >> 1)) * g_height;

    *out.pos_x = vx >> 16;
    *out.pos_y = vy >> 16;
    
    vary->r = vert->r;
    vary->g = vert->g;
    vary->b = vert->b;
    
    vary->u = vert->u, vary->v = vert->v;
    *out.depth = vz;
    *out.depth_inv = (0x7fffffff / vz) << 1;
}

void basic_3d_process_pixel(process_pixel_input_t in, process_pixel_output_t out) {
    basic3d_varying_attr* vary = (basic3d_varying_attr*)in.varying;
    basic3d_uniforms_t* uniforms = (basic3d_uniforms_t*)in.user_uniform;
    int64_t z = in.depth;
    
    int32_t r = vary->r;
    int32_t g = vary->g;
    int32_t b = vary->b;

    int32_t u = vary->u;
    int32_t v = vary->v;
    uint8_t shamt = uniforms->uv_right_shift;
    u >>= shamt;
    v >>= shamt;
    uint8_t col[3];
    //tgfx_rgb(uniforms->tex, u, v, col);
    tgfx_rgb888(uniforms->tex, u, v, col);
    //uint8_t tex = ((u >> 4) + (v >> 4)) % 2 == 0 ? 96 : 255;
    //col[0] = col[1] = col[2] = in.depth >> 8;

    //col[0] = u;
    //col[1] = v;
    //col[2] = 0;

    uint8_t blend_b = (in.depth >> 8) - 1;
    uint8_t blend_a = 255 - blend_b;
    uint8_t col_b[3] = {108, 99, 116};
    for (int i = 0; i < 3; i++) {
        col[i] = ((uint16_t)col[i] * blend_a + (uint16_t)col_b[i] * blend_b) >> 8;
    }
    
    out.rgb[2] = col[0];
    out.rgb[1] = col[1];
    out.rgb[0] = col[2];
}


static const pipeline_processor_t processor_basic_3d = {
        .vertex_size = (3 + 3 + 2) * 4,
        .n_vertex_attrs = 3,
        .n_varying = 2,
        .uniform_fmt = nullptr,
        .vertex_fmt = basic_3d_vert_attr,
        .varying_fmt = basic_3d_varying_attr,
        .process_vertex = basic_3d_process_vertex,
        .process_pixel = basic_3d_process_pixel
};

pipeline_processor_t gen_processor_basic_3d() {
    return processor_basic_3d;
}