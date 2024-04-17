#pragma once

#include <windows.h>
#include <stdlib.h>
#include <stdint.h>


typedef void(*window_render_fn_t)();

extern uint8_t *g_bmp;
extern int32_t g_width, g_height;

bool window_create(HINSTANCE inst, uint32_t width, uint32_t height, uint32_t pixelscale);
void window_destroy();
void window_loop(window_render_fn_t render);
void window_clear_draw_buffer();
void window_swap_buffers();
void window_mousepos_delta(int32_t* x, int32_t* y);
void window_mousepos(int32_t* x, int32_t* y);

static inline void set_pixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b) {
    //if(x < 0 || x >= g_width) return;
    //if(y < 0 || y >= g_height) return;
    unsigned char* pixel = g_bmp + (y * g_width + x) * 3;
    pixel[0] = b;
    pixel[1] = g;
    pixel[2] = r;
}