#include "texture.h"
#include <stdio.h>


tgfx_bmp_res_t tgfx_load_bitmap(uint8_t* raw_buff, texture_t* out) {
	uint32_t pixel_offset = *(uint32_t*)(raw_buff + 0x0a);
	uint32_t width = *(uint32_t*)(raw_buff + 0x12);
	uint32_t height = (*(uint32_t*)(raw_buff + 0x16)) & 0x7fffffff;
	uint16_t bpp = *(uint16_t*)(raw_buff + 0x1c);

	out->width = (uint16_t)width;
	out->height = (uint16_t)height;
	out->data = raw_buff + pixel_offset;

	switch (bpp) {
	case 8:
		out->fmt = TGFX_BMP_BGR8;
		break;
	case 16:
		out->fmt = TGFX_BMP_BGR565;
		break;
	case 24:
		out->fmt = TGFX_BMP_BGR888;
		break;
	default:
		out->fmt = TGFX_BMP_UNKNOWN;
		return TGFX_BMP_INVALID_FORMAT;
	}

	return TGFX_BMP_OK;
}

void tgfx_rgb(texture_t* tex, uint32_t x, uint32_t y, uint8_t* rgb) {
	switch (tex->fmt) {
	case TGFX_BMP_BGR565:
		tgfx_rgb565(tex, x, y, rgb);
		break;
	case TGFX_BMP_BGR888:
		tgfx_rgb888(tex, x, y, rgb);
		break;
	default:
		break;
	}
}