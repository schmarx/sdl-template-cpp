#include "vec.h"
#include <stdio.h>

// -------------------------------------------
// ----- 2-dimensional vector operations -----
// -------------------------------------------

vec2::vec2() {
	this->x = 0;
	this->y = 0;
}

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

void vec2::operator*=(float c) {
	x *= c;
	y *= c;
}

void vec2::operator+=(vec2 v) {
	x += v.x;
	y += v.y;
}

void vec2::operator-=(vec2 v) {
	x -= v.x;
	y -= v.y;
}

float vec2::mag_sqr() {
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

vec3::vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3::~vec3() {
	// nothing to do
}

float vec3::operator*(vec3 v) {
	return x * v.x + y * v.y + z * v.z;
}

vec3 vec3::operator*(float c) {
	return vec3(x * c, y * c, z * c);
}

vec3 vec3::operator+(vec3 v) {
	return vec3(x + v.x, y + v.y, z + v.z);
}
vec3 vec3::operator-(vec3 v) {
	return vec3(x - v.x, y - v.y, z - v.z);
}

void vec3::operator*=(float c) {
	x *= c;
	y *= c;
	z *= c;
}

void vec3::operator+=(vec3 v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void vec3::operator-=(vec3 v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

float vec3::mag_sqr() {
	vec3 v = *this;
	return v * v;
}

vec3 vec3::proj(vec3 v) {
	return v * (((*this) * v) / (v * v));
}

vec3 vec3::cross(vec3 v) {
	return vec3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x);
}

void vec3::print() {
	printf("(%.2f, %.2f, %.2f)\n", x, y, z);
}