#pragma once
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"
#include <stdio.h>

struct mat4 {
	GLfloat x[16];
};

struct vec3 {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct Triangle {
	vec3 v1 = { -1.f, -1.f, 0.f };
	vec3 v2 = { 1.f, -1.f, 0.f };
	vec3 v3 = { 0.f, 1.f, 0.f };
};

struct Quad {
	Triangle t1;
	Triangle t2;
};

struct Transform {
	/* TODO: SOA */
	vec3 pos = { 0.f, 0.f, 0.f };
	vec3 rot = { 0.f, 0.f, 0.f };
	vec3 scale = { 1.f, 1.f, 1.f };
};

struct ParticleData {
	GLsizei size = 0;
	vec3* pos = nullptr;
	vec3* rot = nullptr;
	vec3* scale = nullptr;
	Triangle* triangle = nullptr;

	ParticleData() = delete;
	ParticleData(GLsizei qty) {
		size = qty;
		pos = (vec3*)malloc(size * sizeof(vec3));
		rot = (vec3*)malloc(size * sizeof(vec3));
		scale = (vec3*)malloc(size * sizeof(vec3));
		triangle = (Triangle*)malloc(size * sizeof(Triangle));

		for (int i = 0; i < size; ++i) {
			pos[i] = { 0.f, 0.f, 0.f };
			rot[i] = {};
			scale[i] = { 1.f, 1.f, 1.f };
			triangle[i] = {};
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
		if (triangle) {
			free(triangle);
			triangle = nullptr;
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
