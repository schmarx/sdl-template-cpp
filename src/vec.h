#pragma once

class vec2 {
  private:
  public:
	float x;
	float y;

	vec2(float x, float y); // constructor
	~vec2();				// destructor

	float operator*(vec2 v); // dot product
	vec2 operator*(float c); // scalar multiplication
	vec2 operator+(vec2 v);	 // vector addition
	vec2 operator-(vec2 v);	 // vector subtraction
	float mag();			 // vector norm
	vec2 proj(vec2 v);		 // vector projection

	void print(); // print the vector
};

typedef struct {
	float x;
	float y;
	float z;
} vec3;

float vec3_dot(vec3 v1, vec3 v2);
float vec3_mag(vec3 v);
vec3 vec3_mul(vec3 v, float c);
vec3 vec3_add(vec3 v1, vec3 v2);
vec3 vec3_sub(vec3 v1, vec3 v2);
vec3 vec3_cross(vec3 v1, vec3 v2);
vec3 vec3_proj(vec3 v1, vec3 v2);