#pragma once

#include <stdint.h>

#define TGFX_ASSERT(op) if((op) != TGFX_BMP_OK) for(;;);

typedef enum tgfx_bmp_res_t {
	TGFX_BMP_OK,
	TGFX_BMP_INVALID_FORMAT,
	TGFX_BMP_UNKNOWN_ERR
} tgfx_bmp_res_t;

typedef enum tgfx_bmp_format_t {
	TGFX_BMP_UNKNOWN,
	TGFX_BMP_BGR8,
	TGFX_BMP_BGR565,
	TGFX_BMP_BGR888
} tgfx_bmp_format_t;

struct texture_t {
	uint16_t width;
	uint16_t height;
	tgfx_bmp_format_t fmt;
	uint8_t* data;
};


// Load bitmap
tgfx_bmp_res_t tgfx_load_bitmap(uint8_t* raw_buff, texture_t* out);

void tgfx_rgb(texture_t* tex, uint32_t x, uint32_t y, uint8_t* rgb);

inline static void tgfx_rgb565(texture_t* tex, uint32_t x, uint32_t y, uint8_t *rgb) {
	uint16_t pixel = *(uint16_t*)(tex->data + (y * tex->width + x) * 2);
	rgb[0] = (pixel & 0b11111) << 3;
	rgb[1] = ((pixel >> 5) & 0b111111) << 2;
	rgb[1] = (pixel >> 11) << 3;
}

inline static void tgfx_rgb888(texture_t* tex, uint32_t x, uint32_t y, uint8_t* rgb) {
	uint8_t *pixel = tex->data + (y * tex->width + x) * 3;
	if (x >= tex->width || y >= tex->height) return;
	rgb[0] = *(pixel + 2);
	rgb[1] = *(pixel + 1);
	rgb[2] = *pixel;
}