#include "3dmath.h"


void mul_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *b, mat4_16p16_t *res) {
    res->v[0] = mul_f16p16(a->v[0], b->v[0 + 0]) + mul_f16p16(a->v[1], b->v[4 + 0]) + mul_f16p16(a->v[2], b->v[8 + 0]) + mul_f16p16(a->v[3], b->v[12 + 0]);
    res->v[1] = mul_f16p16(a->v[0], b->v[0 + 1]) + mul_f16p16(a->v[1], b->v[4 + 1]) + mul_f16p16(a->v[2], b->v[8 + 1]) + mul_f16p16(a->v[3], b->v[12 + 1]);
    res->v[2] = mul_f16p16(a->v[0], b->v[0 + 2]) + mul_f16p16(a->v[1], b->v[4 + 2]) + mul_f16p16(a->v[2], b->v[8 + 2]) + mul_f16p16(a->v[3], b->v[12 + 2]);
    res->v[3] = mul_f16p16(a->v[0], b->v[0 + 3]) + mul_f16p16(a->v[1], b->v[4 + 3]) + mul_f16p16(a->v[2], b->v[8 + 3]) + mul_f16p16(a->v[3], b->v[12 + 3]);

    res->v[4] = mul_f16p16(a->v[4], b->v[0 + 0]) + mul_f16p16(a->v[5], b->v[4 + 0]) + mul_f16p16(a->v[6], b->v[8 + 0]) + mul_f16p16(a->v[7], b->v[12 + 0]);
    res->v[5] = mul_f16p16(a->v[4], b->v[0 + 1]) + mul_f16p16(a->v[5], b->v[4 + 1]) + mul_f16p16(a->v[6], b->v[8 + 1]) + mul_f16p16(a->v[7], b->v[12 + 1]);
    res->v[6] = mul_f16p16(a->v[4], b->v[0 + 2]) + mul_f16p16(a->v[5], b->v[4 + 2]) + mul_f16p16(a->v[6], b->v[8 + 2]) + mul_f16p16(a->v[7], b->v[12 + 2]);
    res->v[7] = mul_f16p16(a->v[4], b->v[0 + 3]) + mul_f16p16(a->v[5], b->v[4 + 3]) + mul_f16p16(a->v[6], b->v[8 + 3]) + mul_f16p16(a->v[7], b->v[12 + 3]);

    res->v[8]  = mul_f16p16(a->v[8], b->v[0 + 0]) + mul_f16p16(a->v[9], b->v[4 + 0]) + mul_f16p16(a->v[10], b->v[8 + 0]) + mul_f16p16(a->v[11], b->v[12 + 0]);
    res->v[9]  = mul_f16p16(a->v[8], b->v[0 + 1]) + mul_f16p16(a->v[9], b->v[4 + 1]) + mul_f16p16(a->v[10], b->v[8 + 1]) + mul_f16p16(a->v[11], b->v[12 + 1]);
    res->v[10] = mul_f16p16(a->v[8], b->v[0 + 2]) + mul_f16p16(a->v[9], b->v[4 + 2]) + mul_f16p16(a->v[10], b->v[8 + 2]) + mul_f16p16(a->v[11], b->v[12 + 2]);
    res->v[11] = mul_f16p16(a->v[8], b->v[0 + 3]) + mul_f16p16(a->v[9], b->v[4 + 3]) + mul_f16p16(a->v[10], b->v[8 + 3]) + mul_f16p16(a->v[11], b->v[12 + 3]);

    res->v[12] = mul_f16p16(a->v[12], b->v[0 + 0]) + mul_f16p16(a->v[13], b->v[4 + 0]) + mul_f16p16(a->v[14], b->v[8 + 0]) + mul_f16p16(a->v[15], b->v[12 + 0]);
    res->v[13] = mul_f16p16(a->v[12], b->v[0 + 1]) + mul_f16p16(a->v[13], b->v[4 + 1]) + mul_f16p16(a->v[14], b->v[8 + 1]) + mul_f16p16(a->v[15], b->v[12 + 1]);
    res->v[14] = mul_f16p16(a->v[12], b->v[0 + 2]) + mul_f16p16(a->v[13], b->v[4 + 2]) + mul_f16p16(a->v[14], b->v[8 + 2]) + mul_f16p16(a->v[15], b->v[12 + 2]);
    res->v[15] = mul_f16p16(a->v[12], b->v[0 + 3]) + mul_f16p16(a->v[13], b->v[4 + 3]) + mul_f16p16(a->v[14], b->v[8 + 3]) + mul_f16p16(a->v[15], b->v[12 + 3]);

}

void mul_mat4_16p16_vec4_16p16(mat4_16p16_t *a, vec4_16p16_t *b, vec4_16p16_t *res) {
    res->v[0] = mul_f16p16(a->v[0], b->v[0]) + mul_f16p16(a->v[1], b->v[1]) + mul_f16p16(a->v[2], b->v[2]) + mul_f16p16(a->v[3], b->v[3]);
    res->v[1] = mul_f16p16(a->v[4], b->v[0]) + mul_f16p16(a->v[5], b->v[1]) + mul_f16p16(a->v[6], b->v[2]) + mul_f16p16(a->v[7], b->v[3]);
    res->v[2] = mul_f16p16(a->v[8], b->v[0]) + mul_f16p16(a->v[9], b->v[1]) + mul_f16p16(a->v[10], b->v[2]) + mul_f16p16(a->v[11], b->v[3]);
    res->v[3] = mul_f16p16(a->v[12], b->v[0]) + mul_f16p16(a->v[13], b->v[1]) + mul_f16p16(a->v[14], b->v[2]) + mul_f16p16(a->v[15], b->v[3]);
}

void add_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *b, mat4_16p16_t *res) {
    for(int i = 0; i < 16; i++)
        res->v[i] = a->v[i] + b->v[i];
}
void add_vec4_16p16(vec4_16p16_t *a, vec4_16p16_t *b, vec4_16p16_t *res) {
    for(int i = 0; i < 4; i++)
        res->v[i] = a->v[i] + b->v[i];
}
void add_vec4_16p16_scalar(vec4_16p16_t *a, fp16p16_t b, vec4_16p16_t *res) {
    for(int i = 0; i < 4; i++)
        res->v[i] = a->v[i] + b;
}
void sub_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *b, mat4_16p16_t *res) {
    for(int i = 0; i < 16; i++)
        res->v[i] = a->v[i] - b->v[i];
}
void sub_vec4_16p16(vec4_16p16_t *a, vec4_16p16_t *b, vec4_16p16_t *res) {
    for(int i = 0; i < 4; i++)
        res->v[i] = a->v[i] - b->v[i];
}
void sub_vec4_16p16_scalar(vec4_16p16_t *a, fp16p16_t b, vec4_16p16_t *res) {
    for(int i = 0; i < 4; i++)
        res->v[i] = a->v[i] - b;
}

void mul_vec4_16p16_scalar(vec4_16p16_t *a, fp16p16_t b, vec4_16p16_t *res) {
    for(int i = 0; i < 4; i++)
        res->v[i] = mul_f16p16(a->v[i], b);
}

void mul_mat4_16p16_scalar(mat4_16p16_t *a, fp16p16_t b, mat4_16p16_t *res) {
    for(int i = 0; i < 16; i++)
        res->v[i] = mul_f16p16(a->v[i], b);
}

fp16p16_t det_mat4_16p16(mat4_16p16_t *a) {
    fp16p16_t d = 0;
    d += a->v[0] * a->v[5] * a->v[10] * a->v[15];
    d += a->v[4] * a->v[9] * a->v[14] * a->v[3];
    d += a->v[8] * a->v[13] * a->v[2] * a->v[7];
    d += a->v[12] * a->v[1] * a->v[6] * a->v[11];
    d -= a->v[3] * a->v[6] * a->v[9] * a->v[12];
    d -= a->v[7] * a->v[10] * a->v[13] * a->v[0];
    d -= a->v[11] * a->v[14] * a->v[1] * a->v[4];
    d -= a->v[15] * a->v[2] * a->v[5] * a->v[8];
    return d;
}

void tra_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *res) {
    res->v[0] = a->v[0];
    res->v[1] = a->v[4];
    res->v[2] = a->v[8];
    res->v[3] = a->v[12];

    res->v[4] = a->v[1];
    res->v[5] = a->v[5];
    res->v[6] = a->v[9];
    res->v[7] = a->v[13];

    res->v[8] = a->v[2];
    res->v[9] = a->v[6];
    res->v[10] = a->v[10];
    res->v[11] = a->v[14];

    res->v[12] = a->v[3];
    res->v[13] = a->v[7];
    res->v[14] = a->v[11];
    res->v[15] = a->v[15];
}

void inv_mat4_16p16(mat4_16p16_t *a, mat4_16p16_t *res) {
    fp16p16_t det = det_mat4_16p16(a);
    tra_mat4_16p16(a, res);
    if(det)
        mul_mat4_16p16_scalar(res, div_f16p16(FIXED_UNIT, det), res);
}

mat4_16p16_t idt_mat4_16p16() {
    int32_t unit = 65536;
    return mat4_16p16_t(
        unit, 0, 0, 0,
        0, unit, 0, 0,
        0, 0, unit, 0,
        0, 0, 0, unit
    );
}


void print_mat4_16p16(mat4_16p16_t *a) {
    printf("[\n");
    for(int i = 0; i < 4; i++) {
        printf("\t");
        for(int j = 0; j < 4; j++) {
            fp16p16_t x = a->v[i * 4 + j];
            //printf("%.3f\t", TO_FLOAT(x));
            printf("%.4f\t", TO_FLOAT(x));
        }
        printf("\n");
    }
    printf("]\n");
}

void print_vec4_16p16(vec4_16p16_t *a) {
    printf("[\n\t%.4f\n\t%.4f\n\t%.4f\n\t%.4f\n]\n", TO_FLOAT(a->v[0]), TO_FLOAT(a->v[1]), TO_FLOAT(a->v[2]), TO_FLOAT(a->v[3]));
}

void transform_rotatef(mat4_16p16_t* m, float roll, float pitch, float yaw) {
    /*float sin_alpha = sin(z), cos_alpha = cos(z);
    float sin_beta = sin(y), cos_beta = cos(y);
    float sin_gamma = sin(x), cos_gamma = cos(x);
    int32_t unit = 65536;
    m->v[0] = cos_beta * cos_gamma * unit;
    m->v[1] = (sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma) * unit;
    m->v[2] = (cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma) * unit;
    m->v[3] = 0;
    m->v[4] = (cos_beta * sin_gamma) * unit;
    m->v[5] = (sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma) * unit;
    m->v[6] = (cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma) * unit;
    m->v[7] = 0;
    m->v[8] = -sin_beta * unit;
    m->v[9] = sin_alpha * cos_beta * unit;
    m->v[10] = cos_alpha * cos_beta * unit;
    m->v[11] = 0;
    m->v[12] = 0;
    m->v[13] = 0;
    m->v[14] = 0;
    m->v[15] = unit;*/
    float sin_alpha = sin(yaw), cos_alpha = cos(yaw);
    float sin_beta = sin(pitch), cos_beta = cos(pitch);
    float sin_gamma = sin(roll), cos_gamma = cos(roll);
    int32_t unit = 65536;
    
    m->v[0] = cos_alpha * cos_beta * unit;
    m->v[1] = (cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma) * unit;
    m->v[2] = (cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma) * unit;
    m->v[3] = 0;
    m->v[4] = sin_alpha * cos_beta * unit;
    m->v[5] = (sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma) * unit;
    m->v[6] = (sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma) * unit;
    m->v[7] = 0;
    m->v[8] = -sin_beta * unit;
    m->v[9] = cos_beta * sin_gamma * unit;
    m->v[10] = cos_beta * cos_gamma * unit;
    m->v[11] = 0;
    m->v[12] = 0;
    m->v[13] = 0;
    m->v[14] = 0;
    m->v[15] = unit;
    
    /*m->v[0] = cos_alpha * unit, m->v[1] = sin_alpha * unit * (-1), m->v[2] = 0, m->v[3] = 0;
    m->v[4] = sin_alpha * unit, m->v[5] = cos_alpha * unit, m->v[6] = 0, m->v[7] = 0;
    m->v[8] = 0, m->v[9] = 0, m->v[10] = unit, m->v[11] = 0;
    m->v[12] = 0, m->v[13] = 0, m->v[14] = 0, m->v[15] = unit;*/
}

void transform_translatef(mat4_16p16_t* m, float x, float y, float z) {
    int32_t unit = 65536;
    m->v[0] = unit, m->v[1] = 0, m->v[2] = 0, m->v[3] = x * unit;
    m->v[4] = 0, m->v[5] = unit, m->v[6] = 0, m->v[7] = y * unit;
    m->v[8] = 0, m->v[9] = 0, m->v[10] = unit, m->v[11] = z * unit;
    m->v[12] = 0, m->v[13] = 0, m->v[14] = 0, m->v[15] = unit;
}

void transform_scalef(mat4_16p16_t* m, float x, float y, float z) {
    int32_t unit = 65536;
    m->v[0] = unit * x, m->v[1] = 0, m->v[2] = 0, m->v[3] = 0;
    m->v[4] = 0, m->v[5] = unit * y, m->v[6] = 0, m->v[7] = 0;
    m->v[8] = 0, m->v[9] = 0, m->v[10] = unit * z, m->v[11] = 0;
    m->v[12] = 0, m->v[13] = 0, m->v[14] = 0, m->v[15] = unit;
}

void transform_scalef_all(mat4_16p16_t* m, float s) {
    transform_scalef(m, s, s, s);
}

void transform_perspective(mat4_16p16_t* m, int32_t aspect, int32_t fov, int32_t near, int32_t far) {
    int32_t tf = tanf(fov / 65536.f / 2) * 65536.f;
    m->v[0] = (0x7fffffff / (((int64_t)aspect * tf) >> 16)) << 1;
    m->v[1] = 0;
    m->v[2] = 0;
    m->v[3] = 0;

    m->v[4] = 0;
    m->v[5] = -((0x7fffffff / tf) << 1);
    m->v[6] = 0;
    m->v[7] = 0;

    m->v[8] = 0;
    m->v[9] = 0;
    m->v[10] = -(0x7fffffff / (far - near)) << 1;
    m->v[11] = -((int64_t)(near) << 16) / (far - near);

    m->v[12] = 0;
    m->v[13] = 0;
    m->v[14] = 0x10000;
    m->v[15] = 0;
}

void transform_perspectivef(mat4_16p16_t* m, float aspect, float fov, float near, float far) {
    int32_t unit = 65536;
    float tf = tanf(fov / 2);
    m->v[0] = 1.f / (aspect * tf) * unit;
    m->v[1] = 0;
    m->v[2] = 0;
    m->v[3] = 0;

    m->v[4] = 0;
    m->v[5] = 1.f / tf * unit;
    m->v[6] = 0;
    m->v[7] = 0;

    m->v[8] = 0;
    m->v[9] = 0;
    m->v[10] = 1 / (far - near) * unit;
    m->v[11] = -1 * near / (far - near) * unit;

    m->v[12] = 0;
    m->v[13] = 0;
    m->v[14] = unit;
    m->v[15] = 0;
}


float lerp(float x, float a, float b) {
    return a + x * (b - a);
}
