#pragma once

class vec2 {
  private:
  public:
	float x;
	float y;

	vec2();					// default constructor
	vec2(float x, float y); // constructor
	~vec2();				// destructor

	float operator*(vec2 v);  // dot product
	vec2 operator*(float c);  // scalar multiplication
	void operator*=(float c); // scalar multiplication
	vec2 operator+(vec2 v);	  // vector addition
	void operator+=(vec2 v);  // vector addition
	vec2 operator-(vec2 v);	  // vector subtraction
	void operator-=(vec2 v);  // vector subtraction
	float mag_sqr();		  // vector norm squared
	vec2 proj(vec2 v);		  // vector projection

	void print(); // print the vector
};

class vec3 {
  private:
  public:
	float x;
	float y;
	float z;

	vec3(float x, float y, float z); // constructor
	~vec3();						 // destructor

	float operator*(vec3 v);  // dot product
	vec3 operator*(float c);  // scalar multiplication
	void operator*=(float c); // scalar multiplication
	vec3 operator+(vec3 v);	  // vector addition
	void operator+=(vec3 v);  // vector addition
	vec3 operator-(vec3 v);	  // vector subtraction
	void operator-=(vec3 v);  // vector subtraction
	float mag_sqr();		  // vector norm squared
	vec3 proj(vec3 v);		  // vector projection
	vec3 cross(vec3 v);		  // vector cross product

	void print(); // print the vector
};