#include "vec.h"
#include <stdio.h>

// -------------------------------------------
// ----- 2-dimensional vector operations -----
// -------------------------------------------

vec2::vec2(float x, float y) {
	this->x = x;
	this->y = y;
}

vec2::~vec2() {
	// nothing to do
}

float vec2::operator*(vec2 v) {
	return x * v.x + y * v.y;
}

vec2 vec2::operator*(float c) {
	return vec2(x * c, y * c);
}

vec2 vec2::operator+(vec2 v) {
	return vec2(x + v.x, y + v.y);
}

vec2 vec2::operator-(vec2 v) {
	return vec2(x - v.x, y - v.y);
}

float vec2::mag() {
	vec2 v = *this;
	return v * v;
}

vec2 vec2::proj(vec2 v) {
	return v * (((*this) * v) / (v * v));
}

void vec2::print() {
	printf("(%.2f, %.2f)\n", x, y);
}

// -------------------------------------------
// ----- 3-dimensional vector operations -----
// -------------------------------------------

float vec3_dot(vec3 v1, vec3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vec3_mag(vec3 v) {
	return vec3_dot(v, v);
}

vec3 vec3_mul(vec3 v, float c) {
	vec3 result = {c * v.x, c * v.y, c * v.z};
	return result;
}

vec3 vec3_add(vec3 v1, vec3 v2) {
	vec3 result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	return result;
}

vec3 vec3_sub(vec3 v1, vec3 v2) {
	vec3 result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	return result;
}

vec3 vec3_cross(vec3 v1, vec3 v2) {
	vec3 result = {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x};

	return result;
}

// project v1 onto v2
vec3 vec3_proj(vec3 v1, vec3 v2) {
	return vec3_mul(v2, vec3_dot(v1, v2) / vec3_dot(v2, v2));
}