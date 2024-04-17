#pragma once

#define TO_FIXED(val) ((int32_t)(val * 65536.f))
#define TO_FLOAT(val) ((float)(val) / 65536.f)
#define TO_INT(val) ((val) / 65536)
#define FIXED_UNIT 65536

#include <stdio.h>
#include <stdint.h>
#include <math.h>


typedef int32_t fp16p16_t;


struct mat4_16p16_t {
    fp16p16_t v[16];

    mat4_16p16_t() {
        for(int i = 0; i < 16; i++)
            v[i] = 0;
    }
    mat4_16p16_t(
        fp16p16_t a00, fp16p16_t a01, fp16p16_t a02, fp16p16_t a03,
        fp16p16_t a10, fp16p16_t a11, fp16p16_t a12, fp16p16_t a13,
        fp16p16_t a20, fp16p16_t a21, fp16p16_t a22, fp16p16_t a23,
        fp16p16_t a30, fp16p16_t a31, fp16p16_t a32, fp16p16_t a33
    ) {
        v[0] = a00, v[1] = a01, v[2] = a02, v[3] = a03;
        v[4] = a10, v[5] = a11, v[6] = a12, v[7] = a13;
        v[8] = a20, v[9] = a21, v[10] = a22, v[11] = a23;
        v[12] = a30, v[13] = a31, v[14] = a32, v[15] = a33;
    }
};

struct vec4_16p16_t {
    fp16p16_t v[4];

    vec4_16p16_t() {
        for(int i = 0; i < 4; i++)
            v[i] = 0;
    }

    vec4_16p16_t(fp16p16_t a00, fp16p16_t a10, fp16p16_t a20, fp16p16_t a30) {
        v[0] = a00, v[1] = a10, v[2] = a20, v[3] = a30;
    }
};

void mul_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *b, mat4_16p16_t *res);
void mul_mat4_16p16_vec4_16p16(mat4_16p16_t *a, vec4_16p16_t *b, vec4_16p16_t *res);

fp16p16_t det_mat4_16p16(mat4_16p16_t *a);
void tra_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *res);
void inv_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *res);
mat4_16p16_t idt_mat4_16p16();

void add_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *b, mat4_16p16_t *res);
void add_vec4_16p16(vec4_16p16_t *a, vec4_16p16_t *b, vec4_16p16_t *res);
void add_vec4_16p16_scalar(vec4_16p16_t *a, fp16p16_t b, vec4_16p16_t *res);
void sub_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *b, mat4_16p16_t *res);
void sub_vec4_16p16(vec4_16p16_t *a, vec4_16p16_t *b, vec4_16p16_t *res);
void sub_vec4_16p16_scalar(vec4_16p16_t *a, fp16p16_t b, vec4_16p16_t *res);
void mul_mat4_16p16_scalar(mat4_16p16_t *a, fp16p16_t b, mat4_16p16_t *res);
void mul_vec4_16p16_scalar(vec4_16p16_t *a, fp16p16_t b, vec4_16p16_t *res);

void print_mat4_16p16(mat4_16p16_t *a);
void print_vec4_16p16(vec4_16p16_t *a);


void transform_rotatef(mat4_16p16_t* m, float x, float y, float z);
void transform_translatef(mat4_16p16_t* m, float x, float y, float z);
void transform_scalef(mat4_16p16_t* m, float x, float y, float z);
void transform_scalef_all(mat4_16p16_t* m, float s);
void transform_perspectivef(mat4_16p16_t* m, float right, float top, float near, float far);

void transform_perspective(mat4_16p16_t* m, int32_t aspect, int32_t fov, int32_t near, int32_t far);

inline fp16p16_t mul_f16p16(fp16p16_t a, fp16p16_t b) {
    return (((int64_t)a) * b) >> 16;
}

inline fp16p16_t div_f16p16(fp16p16_t a, fp16p16_t b) {
    return (((int64_t)a) << 16) / b;
}


float lerp(float x, float a, float b);
