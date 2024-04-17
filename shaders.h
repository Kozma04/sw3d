#pragma once

#include <cstdint>
#include "pipeline.h"
#include "3dmath.h"
#include "window.h"
#include "texture.h"


struct basic3d_uniforms_t {
	texture_t* tex;
	mat4_16p16_t mvp;
	uint8_t uv_right_shift; // amount to right shift u and v to get texture coordinates
};


pipeline_processor_t gen_processor_basic_3d();