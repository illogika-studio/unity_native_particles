#pragma once
#include "particle_manager.h"

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
	unsigned int pipeline_id = 0;
	int vert_shader_id = 0;
	int frag_shader_id = 0;

	ParticleManager* _particle_manager = nullptr;
};
