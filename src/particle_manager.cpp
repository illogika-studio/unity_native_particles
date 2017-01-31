#include "particle_manager.h"
#include "globals.h"

#include <stdio.h>
//#include <stdlib.h>

const GLfloat test_tri[9] = {
	-0.5f, -0.5f, 0.f
	, 0.5f, -0.5f, 0.f
	, 0.f, 0.5f, 0.f
};

ParticleManager::ParticleManager(GLsizei qty)
{
	if (qty == 0) {
		OUTPUT_ERROR("Please set the quantity of particles desired.\n");
		return;
	}
	if (!(_transforms = static_cast<Transform*>(malloc(qty * sizeof(Transform))))) {
		OUTPUT_ERROR("Could not allocate transform memory.\n");
		return;
	}
	if (!(_triangles = static_cast<Triangle*>(malloc(qty * sizeof(Triangle))))) {
		OUTPUT_ERROR("Could not allocate triangle memory.\n");
		return;
	}

	for (size_t i = 0; i < qty; ++i) {
		_transforms[i] = Transform{};
		_triangles[i] = Triangle{};
	}

	oglGenVertexArrays(1, &vertex_array_id);
	oglBindVertexArray(vertex_array_id);

	oglGenBuffers(1, &vertex_buffer_id);
	oglBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	oglBufferData(GL_ARRAY_BUFFER, sizeof(test_tri),
			test_tri, GL_STATIC_DRAW);
	oglEnableVertexAttribArray(0);
	oglVertexAttribPointer(
			0,			// Must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void*)0	// array buffer offset
	);
	oglBindVertexArray(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
		printf("OpenGL error: %d", err);
    }
	_initialized = true;
}

ParticleManager::~ParticleManager()
{
	if (_transforms) {
		free(_transforms);
		_transforms = nullptr;
	}
	if (_triangles) {
		free(_triangles);
		_triangles = nullptr;
	}
	_qty = 0;
}

void ParticleManager::render()
{
	if (!_initialized)
		return;

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	//glClearColor(1.f, 0.8f, 0.f, 1.f);
	//glClear( GL_COLOR_BUFFER_BIT );
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glRects(-1,-1,1,1);
	glDrawArrays(GL_TRIANGLES, GL_POINTS, 9);
}
