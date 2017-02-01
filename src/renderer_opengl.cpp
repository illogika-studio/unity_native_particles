#include "renderer_opengl.h"
#include "shader.h"
#include "globals.h"

#include <stdio.h>

void* gl_funcs[NUMFUNCS];

#define fzn  0.005f
#define fzf  1000.0f

static const float projectionmatrix[16] = {
    1.0f, 0.00f,  0.0f,                    0.0f,
    0.0f, 1.25f,  0.0f,                    0.0f,
    0.0f, 0.00f, -(fzf+fzn)/(fzf-fzn),    -1.0f,
    0.0f, 0.00f, -2.0f*fzf*fzn/(fzf-fzn),  0.0f
};

static const GLfloat test_tri[9] = {
	-0.5f, -0.5f, 0.f
	, 0.5f, -0.5f, 0.f
	, 0.f, 0.5f, 0.f
};

Renderer::Renderer(GLsizei particle_qty)
{
	if (particle_qty == 0) {
		OUTPUT_ERROR("Please set the quantity of particles desired.\n");
		return;
	}
	_particle_qty = particle_qty;

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
		
		oglBindFragDataLocation(frag_shader_id, 0, "fragColor");
		gl_has_error();

		oglGenProgramPipelines(1, &pipeline_id);
		oglBindProgramPipeline(pipeline_id);
		oglUseProgramStages(pipeline_id, GL_VERTEX_SHADER_BIT, vert_shader_id);
		oglUseProgramStages(pipeline_id, GL_FRAGMENT_SHADER_BIT, frag_shader_id);

		int was_init = 1;
		was_init *= gl_was_initialized(pipeline_id);
		was_init *= gl_was_initialized(vert_shader_id);
		was_init *= gl_was_initialized(frag_shader_id);

		if (!was_init) {
			OUTPUT_ERROR("Problem initializing OpenGL. Will not render anything.\n");
			return;
		}
	}

	/* Memory and Particles */
	{
		printf("transform size : %zd\n", sizeof(Transform));
		printf("triangle size : %zd\n", sizeof(Triangle));
		if (!(_transforms = static_cast<Transform*>(malloc(_particle_qty * sizeof(Transform))))) {
			OUTPUT_ERROR("Could not allocate transform memory.\n");
			return;
		}
		if (!(_triangles = static_cast<Triangle*>(malloc(_particle_qty * sizeof(Triangle))))) {
			OUTPUT_ERROR("Could not allocate triangle memory.\n");
			return;
		}

		for (size_t i = 0; i < _particle_qty; ++i) {
			_transforms[i] = Transform();
			_triangles[i] = Triangle();
		}

		for (size_t i = 0; i < _particle_qty; ++i) {
			printf("%f\n", _triangles[i].v1.x);
		}

		oglGenVertexArrays(1, &vertex_array_id);
		oglBindVertexArray(vertex_array_id);

		oglGenBuffers(1, &vertex_buffer_id);
		oglBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		oglBufferData(GL_ARRAY_BUFFER, sizeof(test_tri),
				test_tri, GL_STATIC_DRAW);

		oglVertexAttribPointer(
				0,			// Must match the layout in the shader.
				3,			// size
				GL_FLOAT,	// type
				GL_FALSE,	// normalized?
				3 * sizeof(GLfloat),			// stride
				(void*)0	// array buffer offset
		);
		oglEnableVertexAttribArray(0);

		if (gl_has_error())
			return;

	}

	_initialized = true;
}

Renderer::~Renderer()
{
	if (_transforms) {
		free(_transforms);
		_transforms = nullptr;
	}
	if (_triangles) {
		free(_triangles);
		_triangles = nullptr;
	}
	_particle_qty = 0;

}

void Renderer::render(float dt)
{
	if (!_initialized)
		return;

	/* Basic render state. */
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);


	oglUseProgram(vert_shader_id);
	oglUseProgram(frag_shader_id);

	/* VAO */
	oglGenVertexArrays(1, &vertex_array_id);
	oglBindVertexArray(vertex_array_id);

	/* Vertex Buffer */
	oglBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	//oglBufferData(GL_ARRAY_BUFFER, sizeof(test_tri),
			//test_tri, GL_STATIC_DRAW);

	oglEnableVertexAttribArray(0);
	oglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glDrawArrays(GL_TRIANGLES, GL_POINTS, 3);
	oglDisableVertexAttribArray(0);
	oglDeleteVertexArrays(1, &vertex_array_id);

}

int Renderer::init_gl_funcs()
{
	for (int i = 0; i < NUMFUNCS; ++i) {
		gl_funcs[i] = wglGetProcAddress(gl_func_names[i]);
		if (!gl_funcs[i]) {
			OUTPUT_ERROR("%s was null.", gl_func_names[i]);
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

