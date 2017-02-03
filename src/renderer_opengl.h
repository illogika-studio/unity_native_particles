#pragma once
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"
#include <stdio.h>

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
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.y };
	}

	inline friend vec3 operator*(float lhs, const vec3& rhs) {
		return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.y };
	}

	inline friend vec3 operator*(const vec3& rhs, float lhs) {
		return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.y };
	}
};

static const GLfloat quad[12] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
};

struct ParticleData {
	GLsizei size = 0;

	const vec3 gravity = { 0.f, -9.8f, 0.f };
	const float spread = 1.5f;
	const float global_speed = 0.1f;

	vec3* pos = nullptr;
	vec3* rot = nullptr;
	vec3* scale = nullptr;
	vec3* speed = nullptr;

	ParticleData() = delete;
	ParticleData(GLsizei qty) {
		size = qty;
		pos = (vec3*)malloc(size * sizeof(vec3));
		rot = (vec3*)malloc(size * sizeof(vec3));
		scale = (vec3*)malloc(size * sizeof(vec3));
		speed = (vec3*)malloc(size * sizeof(vec3));

		srand(209384); // Intentional, use deterministic randomness. TODO: gpu_rand()
		float r = 0.f;
		vec3 main_dir = { 0.f, 10.f, 0.f };
		for (int i = 0; i < size; ++i) {
			r = (rand() % 2000 - 1000.f) / 1000.f;
			r /= 2.f;

			pos[i] = { 0.f, 0.f, 0.f };
			rot[i] = {};
			scale[i] = { 0.25f + r, 0.25f + r, 1.f };
			speed[i] = main_dir + vec3{
				(rand() % 2000 - 1000.f) / 1000.f,
				(rand() % 2000 - 1000.f) / 1000.f,
				(rand() % 2000 - 1000.f) / 1000.f
			} * spread;
		}
	}

	~ParticleData() {
		size = 0;
		if (pos) {
			free(pos);
			pos = nullptr;
		}
		if (rot) {
			free(rot);
			rot = nullptr;
		}
		if (scale) {
			free(scale);
			scale = nullptr;
		}
		if (speed) {
			free(speed);
			speed = nullptr;
		}
	}

	void print(size_t i) {
		printf("%f %f %f\n%f %f %f\n%f %f %f\n\n",
			pos[i].x, pos[i].y, pos[i].z,
			rot[i].x, rot[i].y, rot[i].z,
			scale[i].x, scale[i].y, scale[i].z
		);
	}

	//void get_array(size_t i, GLfloat* output) {
	//	output = { pos[i].x, pos[i].y, pos[i].z
	//		, rot[i].x, rot[i].y, rot[i].z
	//		, scale[i].x, scale[i].y, scale[i].z
	//	};
	//}
};

class Renderer {
public:
	Renderer() = delete;
	Renderer(GLsizei particle_qty);
	~Renderer();

	void init_opengl();
	void detroy_opengl();

	void update(float time, float delta_time);
	void render(float time, float delta_time);

	void mvp(GLfloat m[16], GLfloat v[16], GLfloat p[16]) {
		memcpy(_model_mat, m, sizeof(GLfloat) * 16);
		memcpy(_view_mat, v, sizeof(GLfloat) * 16);
		memcpy(_projection_mat, p, sizeof(GLfloat) * 16);
	}
private:
	inline int init_gl_funcs();

	bool _initialized = false;
	ParticleData* _data = nullptr;
	//	Transform* _transforms = nullptr;
	//	Triangle* _triangles = nullptr;

	GLuint _pipeline_id = 0;
	GLuint _vert_shader_id = 0;
	GLuint _frag_shader_id = 0;
	GLuint _vertex_array_id = 0;
	GLuint _vertex_buffer_id = 0;
	GLint _transform_uniform_id = 0;
	GLint _model_uniform_id = 0;
	GLint _view_uniform_id = 0;
	GLint _projection_uniform_id = 0;
	GLint _time_uniform_id = 0;

	GLfloat _model_mat[16] = {};
	GLfloat _view_mat[16] = {};
	GLfloat _projection_mat[16] = {};

	/* TODO: Expose these. */
	const float _velocity = 0.01f;
};
