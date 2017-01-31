#pragma once

#include <windows.h>
#include <GL/gl.h>

struct mat4 {
	GLfloat x[16];
};

struct vec3 {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

class ParticleManager {
public:
	ParticleManager() = delete;
	ParticleManager(GLsizei qty);
	~ParticleManager();

	void render();

private:
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

	bool _initialized = false;
	Transform* _transforms = nullptr;
	Triangle* _triangles = nullptr;

	GLsizei _qty = 0;
	GLuint vertex_array_id = 0;
	GLuint vertex_buffer_id = 0;
};


