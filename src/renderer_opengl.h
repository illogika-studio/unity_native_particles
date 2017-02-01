#pragma once
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"

struct mat4 {
	GLfloat x[16];
};

struct vec3 {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct Triangle {
	vec3 v1 = {-0.5f, -0.5f, 0.f};
	vec3 v2 = {-0.5f, -0.5f, 0.f};
	vec3 v3 = {-0.5f, -0.5f, 0.f};
};

struct Quad {
	Triangle t1;
	Triangle t2;
};

struct Transform {
	/* TODO: SOA */
	vec3 pos = {0.f, 0.f, 0.f};
	vec3 rot = {0.f, 0.f, 0.f};
	vec3 scale = {1.f, 1.f, 1.f};
//		const GLfloat verts[ParticleVertexQty] = {
//			-0.5f, -0.5f, 0.f
//			, 0.5f, -0.5f, 0.f
//			, 0.f, 0.5f, 0.f
//		};
};

class Renderer {
public:
	Renderer() = delete;
	Renderer(GLsizei particle_qty);
	~Renderer();

	void render(float dt);

private:
	inline int init_gl_funcs();
	inline int gl_was_initialized(int id);

	bool _initialized = false;
	GLsizei _particle_qty = 0;
	Transform* _transforms = nullptr;
	Triangle* _triangles = nullptr;

	GLuint pipeline_id = 0;
	GLuint vert_shader_id = 0;
	GLuint frag_shader_id = 0;
	GLuint vertex_array_id = 0;
	GLuint vertex_buffer_id = 0;

};
