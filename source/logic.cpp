#include <haven_def.h>
#include <math.h>

const float getXpos3d(int idx, float max_x) {
	idx %= (int)max_x;
	return (idx);
}

const float getYpos3d(int idx, float max_x, float max_y) {
	idx /= max_x;
	idx %= (int)max_y;
	return (idx);
}

const float getZpos3d(int idx, float max_x, float max_y) {
	idx /= max_x;
	idx /= max_y;
	return (idx);
}

const float getXpos(const u32 idx, const int width) {
	return (idx % (width));
}

const float getYpos(const u32 idx, const int width) {
	return (floor((float)idx / (width)));
}
const u32 getLinearIndex(const float x, const float y, const int width) {
	return (x + y * width);
}

i32 gcd(i32 a, i32 b) {
	while (1) {
		if (b == 0) {
			break;
		}
		a ^= b;
		b ^= a;
		a ^= b;
		b %= a;
	}
	return (a);
}

f32 rsqrt(f32 num) {
	long i;
	f32 x2, y;
	const f32 threehalfs = 1.5f;

	x2 = num * 0.5f;
	y = num;
	i = *(long *) &y;
	i = 0x5f3759df - (i >> 1);
	y = *(f32 *) &i;
	y = y * (threehalfs - (x2 * y * y));

	return y;
}

f64 power(f64 number, long power) {
	f64 result = 1;
	while (power > 0) {
		if (power & 1) {
			result *= (result * number);
		}
		number = (number * number);
		power >>= 1;
	}
	return (result);
}

float lerpFun(float start, float end, float amount) {
    float result = start + amount*(end - start);

    return (result);
}

f32 smoothStep(f32 t) {
    f32 v1 = t * t;
    f32 v2 = 1.0f - (1.0f - t) * (1.0f - t);
    return (lerpFun(v1, v2, t));
}

f32 clamp(f32 value, f32 lo, f32 hi) {
	if (value < lo) {
		return (lo);
	}
	if (value > hi) {
		return (hi);
	}
	return (value);
}
