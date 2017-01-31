#include "renderer_opengl.h"
#include "shader.h"
#include "globals.h"

#include <windows.h>
#include <GL/gl.h>
#include "glext.h"
#include <stdio.h>

 void* gl_funcs[NUMFUNCS];

Renderer::Renderer(GLsizei particle_qty)
{
	/* OpenGL */
	{
		if (!init_gl_funcs()) {
			OUTPUT_ERROR("Could not get OpenGL functions.\n");
			return;
		}

		vert_shader_id = oglCreateShaderProgramv(GL_VERTEX_SHADER, 1,
				&vert_shader_src);
		frag_shader_id = oglCreateShaderProgramv(GL_FRAGMENT_SHADER, 1,
				&frag_shader_src);

		oglGenProgramPipelines(1, &pipeline_id);
		oglBindProgramPipeline(pipeline_id);
		oglUseProgramStages(pipeline_id, GL_VERTEX_SHADER_BIT, vert_shader_id);
		oglUseProgramStages(pipeline_id, GL_FRAGMENT_SHADER_BIT, frag_shader_id);

		int was_init = 1;
		was_init &= gl_was_initialized(pipeline_id);
		was_init &= gl_was_initialized(vert_shader_id);
		was_init &= gl_was_initialized(frag_shader_id);

		if (!was_init) {
			OUTPUT_ERROR("Problem initializing OpenGL. Will not render anything.\n");
			return;
		}
	}

	/* Memory and Particles */
	{
		if (!(_particle_manager = new ParticleManager(particle_qty))) {
			OUTPUT_ERROR("Couldn't create ParticleManager.\n");
			return;
		}
	}

	_initialized = true;
}

Renderer::~Renderer()
{
	if (_particle_manager) {
		delete _particle_manager;
		_particle_manager = nullptr;
	}
}

void Renderer::render(float dt)
{
	if (!_initialized)
		return;

	_particle_manager->render();
}

int Renderer::init_gl_funcs()
{
	for (int i = 0; i < NUMFUNCS; ++i) {
		gl_funcs[i] = wglGetProcAddress(gl_func_names[i]);
		if (!gl_funcs[i]) {
			printf("%s was null\n.", gl_func_names[i]);
			return 0;
		}
	}
	return 1;
}

int Renderer::gl_was_initialized(int id)
{
	int result;
	char info[1536];
	oglGetProgramiv(id, GL_LINK_STATUS, &result);
	oglGetProgramInfoLog(id, 1024, NULL, (char *)info);
	if(!result) {
		OUTPUT_ERROR("OpenGL initialization error : %s\n", info);
	}
	return result;
}

