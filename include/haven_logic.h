#ifndef HAVEN_LOGIC
# define HAVEN_LOGIC

#include "haven_def.h"
#include <math.h>

const float getXpos3d(int idx, float max_x);
const float getYpos3d(int idx, float max_x, float max_y);
const float getZpos3d(int idx, float max_x, float max_y);
const float getXpos(const u32 idx, const int width);
const float getYpos(const u32 idx, const int width);
const u32 getLinearIndex(const float x, const float y, const int width);
i32 gcd(i32 a, i32 b);
f32 rsqrt(f32 num);
f64 power(f64 number, long power);
float lerpFun(float start, float end, float amount);
f32 smoothStep(f32 t);
f32 clamp(f32 value, f32 lo, f32 hi);

#endif
