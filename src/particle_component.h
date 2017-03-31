#pragma once
#include "globals.h"

#include <windows.h>
#include <GL/gl.h>
#include "glext.h"

#include <utility>

struct vec3 {
	GLfloat x;
	GLfloat y;
	GLfloat z;

	inline vec3& operator+=(const vec3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	inline friend vec3 operator+(vec3 lhs, const vec3& rhs) {
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

	inline friend vec3 operator*(float lhs, const vec3& rhs) {
		return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
	}

	inline friend vec3 operator*(const vec3& rhs, float lhs) {
		return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
	}
};

struct soa_vec3 {
	struct vec3_view {
		GLfloat& x;
		GLfloat& y;
		GLfloat& z;
	};

	GLfloat* x = nullptr;
	GLfloat* y = nullptr;
	GLfloat* z = nullptr;

	~soa_vec3()
	{
		x = nullptr;
		y = nullptr;
		z = nullptr;
	}

	vec3_view operator[](size_t idx) {
		return {x[idx], y[idx], z[idx]};
	}

	static size_t element_byte_size() {
		return sizeof(GLfloat);
	}

	static size_t struct_byte_size() {
		OUTPUT_MSG("SIZEOF Vec3 : %zu", sizeof(decltype(*x)) + sizeof(decltype(*y)) + sizeof(decltype(*z)));
		return sizeof(decltype(*x)) + sizeof(decltype(*y)) + sizeof(decltype(*z));
	}
};

namespace ParticleComponent {
	void init(GLsizei qty);
	void update(float delta_time);

	size_t size();
	size_t total_byte_size();
	void* heap();
	soa_vec3& pos();
	soa_vec3& rot();
	soa_vec3& scale();
	soa_vec3& speed();
};