#include "particle_manager.h"
#include "globals.h"

#include <stdio.h>
//#include <stdlib.h>

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


	oglGenVertexArrays(1, &vertex_array_id);
	oglBindVertexArray(vertex_array_id);

	oglGenBuffers(1, &vertex_buffer_id);
	oglBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	oglBufferData(GL_ARRAY_BUFFER, qty * sizeof(Triangle),
			_triangles, GL_STATIC_DRAW);

	oglEnableVertexAttribArray(0);
	oglBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	oglVertexAttribPointer(
			0,			// Must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void*)0	// array buffer offset
	);

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


	glDrawArrays(GL_TRIANGLES, GL_POINTS, _qty * 3 * 3);
	printf("drawing?");
}
