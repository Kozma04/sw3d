#pragma once

#include "3dmath.h"
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <malloc.h>
#include "utils.h"


struct vec2_t { float x, y; };
struct vec3_t { float x, y, z; };

typedef enum tgfx_feature_enum {
    // Perspective-correct primitive parameters interpolation (including depth)
    TGFX_PERSPECTIVE_DIVIDE = 1 << 0,
    // Enable backface culling
    TGFX_CULL_FACE          = 1 << 1,
    // Cull triangles with vertices arranged in clockwise order if enabled, otherwise counter-clockwise.
    // TGFX_CULL_FACE must be enabled as well.
    TGFX_CULL_FACE_CW       = 1 << 2,
    // Enable writing to the depth buffer
    TGFX_DEPTH_WRITE        = 1 << 3,
    // Interpolate depth across primitives.
    TGFX_DEPTH_CALC         = 1 << 4,
    // Discard pixels with depth >= depth buffer value. TGFX_DEPTH_TEST must be enabled as well.
    TGFX_DEPTH_TEST_LOWER   = 1 << 5,
    // Discard pixels with depth <= depth buffer value. TGFX_DEPTH_TEST must be enabled as well.
    TGFX_DEPTH_TEST_HIGHER  = 1 << 6,
    // Discard pixels with depth outside the interval specified with tgfx_depth_clip
    TGFX_DEPTH_CLIP        = 1 << 7
} tgfx_feature_t;

struct process_vertex_input_t {
    const void* const user_attr; // user-defined attributes for the input vertex (position, color etc.)
    const void* const user_uniform; // user-defined, shader-wide parameters
};

struct process_vertex_output_t {
    int32_t* const pos_x; // output vertex position as a pixel on the surface
    int32_t* const pos_y; // output vertex position as a pixel on the surface
    int32_t* const depth; // vertex Z-value
    int32_t* const depth_inv; // vertex Z-value to be used for perspective divide
    void* const varying_out; // data to be interpolated across the primitive and passed to the pixel processor
};

struct process_pixel_input_t {
    const int32_t pos_x; // pixel position on the surface
    const int32_t pos_y; // pixel position on the surface
    const int32_t const depth;
    const void* const varying; // interpolated vertex attributes
    const void* const user_uniform; // user-defined, shader-wide parameters
};

struct process_pixel_output_t {
    //int32_t* const depth; // depth to be stored in the pixel's location in the z-buffer
    uint8_t* const rgb; // pixel color [R, G, B]
};


typedef void (*process_vertex_fn_t)(process_vertex_input_t in, process_vertex_output_t out);
typedef void (*process_pixel_fn_t)(process_pixel_input_t in, process_pixel_output_t out);


enum attr_type_t {
    PIPELINE_ATTR_TYPE_I32 = 0,
    PIPELINE_ATTR_TYPE_FLT32 = 1,
    PIPELINE_ATTR_TYPE_FIX_S15P16 = 2
};

static const uint32_t g_attr_type_size[] = {
        4, 4, 4
};

struct pipeline_attr_t {
    uint8_t count;
    attr_type_t type; // only float for nou
};

struct pipeline_varying_attr_t {
    uint8_t count;
    //attr_type_t type; // only int32_t for nou
    uint8_t interpolate;
};

/*
 * For a given pipeline_processor_t, the pipeline takes a data buffer as input and generates pixels on the surface as output.
 * Steps performed for 3 consecutive chunks of size vertex_size:
 * 1. Run process_vertex for the next 3 input chunks of the buffer, located at the vertex pointer
 * 2. Draw a triangle with the vertices outputted by the 3 calls to process_vertex
 *    For each triangle pixel:
 *      2.1. Interpolate the "varying" attributes between the 3 vertices
 *      2.2. Call process_pixel
 *      2.3. Store the process_pixel output RGB pixel color to the surface and the depth to the z-buffer
 * 3. Increment vertex pointer by 3 vertices and repeat if the buffer still has min. 3 vertices left to be read
 */
struct pipeline_processor_t {
    // vertex size in bytes
    uint8_t vertex_size;
    // single vertex attribute count (process_vertex input)
    uint8_t n_vertex_attrs;
    // primitive-interpolated parameters count (process_pixel input)
    uint8_t n_varying;
    // format of the uniform buffer, n_uniforms entries
    pipeline_attr_t *uniform_fmt;
    // format of the vertex buffer, n_vertex_attrs entries
    pipeline_attr_t *vertex_fmt;
    // format of the primitive-interpolated parameters, n_varying entries
    pipeline_varying_attr_t *varying_fmt;

    // vertex processing function pointer
    process_vertex_fn_t process_vertex;
    // pixel processing function pointer
    process_pixel_fn_t process_pixel;

    // uniform buffer
    //void *uniforms;
};


void tgfx_init_surface(int32_t width, int32_t height, uint8_t* grb_buf, uint16_t* depth_buf);

void tgfx_enable(tgfx_feature_t feature);
void tgfx_disable(tgfx_feature_t feature);
void tgfx_depth_clip(uint16_t clip_min, uint16_t clip_max);

void tgfx_proc_load(const pipeline_processor_t *proc);
void tgfx_proc_unload();
void tgfx_set_proc_uniform_buf(void* buff);

void tgfx_draw_tri(void* buff);
void tgfx_draw_tris(void* vertex_buf, uint16_t n_tris);
void tgfx_draw_tris_idx(void* vertex_buf, uint16_t* index_buf, uint16_t n_tris);
