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
		OUTPUT_MSG("OpenGL version : %s", glGetString(GL_VERSION));
		//OUTPUT_MSG("OpenGL supported extensions :\n%s", glGetString(GL_EXTENSIONS));
		{
			char err[256] = "";
			gl_error_string(err);
			if (strlen(err) != 0) {
				OUTPUT_ERROR("OpenGL error : %s", err);
				return;
			}
		}

		_vert_shader_id = oglCreateShaderProgramv(GL_VERTEX_SHADER, 1,
				&vert_shader_src);
		_frag_shader_id = oglCreateShaderProgramv(GL_FRAGMENT_SHADER, 1,
				&frag_shader_src);

		{
			char err[256] = "";
			gl_error_string(err);
			if (strlen(err) != 0) {
				OUTPUT_ERROR("OpenGL error : %s", err);
				return;
			}
		}

		oglGenProgramPipelines(1, &_pipeline_id);
		oglBindProgramPipeline(_pipeline_id);
		oglUseProgramStages(_pipeline_id, GL_VERTEX_SHADER_BIT, _vert_shader_id);
		oglUseProgramStages(_pipeline_id, GL_FRAGMENT_SHADER_BIT, _frag_shader_id);

		int was_init = 1;
		was_init *= gl_program_was_initialized(_vert_shader_id);
		was_init *= gl_program_was_initialized(_frag_shader_id);
		was_init *= gl_program_was_initialized(_pipeline_id);

		if (!was_init) {
			OUTPUT_ERROR("Problem initializing OpenGL. Will not render anything.\n");
			return;
		}
		
		//_transform_mat_id = oglGetUniformLocation(_pipeline_id, "transform");
		//if (_transform_mat_id == -1) OUTPUT_ERROR("Problem getting uniform location.");

		_model_mat_id = oglGetUniformLocation(_vert_shader_id, "model_mat");
		if (_model_mat_id == -1) OUTPUT_ERROR("Problem getting uniform location.");

		_view_mat_id = oglGetUniformLocation(_vert_shader_id, "view_mat");
		if (_view_mat_id == -1) OUTPUT_ERROR("Problem getting uniform location.");

		_projection_mat_id = oglGetUniformLocation(_vert_shader_id, "proj_mat");
		if (_projection_mat_id == -1) OUTPUT_ERROR("Problem getting uniform location.");
	}

	/* Memory and Particles */
	{
		_data = new ParticleData(_particle_qty);

		oglGenVertexArrays(1, &_vertex_array_id);
		oglBindVertexArray(_vertex_array_id);

		oglGenBuffers(1, &_vertex_buffer_id);
		oglBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);
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
		/* Cleanup */
		oglEnableVertexAttribArray(0);
		oglBindVertexArray(0);

		char err[256] = "";
		gl_error_string(err);
		if (strlen(err) != 0) {
			OUTPUT_ERROR("OpenGL error : %s", err);
			return;
		}

	}

	_initialized = true;
}

Renderer::~Renderer()
{
	//if (_transforms) {
	//	free(_transforms);
	//	_transforms = nullptr;
	//}
	//if (_triangles) {
	//	free(_triangles);
	//	_triangles = nullptr;
	//}
	_particle_qty = 0;

}

void Renderer::render(float dt)
{
	if (!_initialized)
		return;

	/* Basic render state. */
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_BLEND);
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);


	oglBindProgramPipeline(_pipeline_id);
	{
		char err[256] = "";
		gl_error_string(err);
		if (strlen(err) != 0) {
			OUTPUT_ERROR("OpenGL error : %s", err);
			return;
		}
	}
	oglProgramUniformMatrix4fv(_vert_shader_id, _model_mat_id, 1, GL_FALSE, _model_mat);
	oglProgramUniformMatrix4fv(_vert_shader_id, _view_mat_id, 1, GL_FALSE, _view_mat);
	oglProgramUniformMatrix4fv(_vert_shader_id, _projection_mat_id, 1, GL_FALSE, _projection_mat);
	//oglProgramUniformMatrix3fv(_vert_shader_id, _transform_mat_id, 1, GL_FALSE, todo);
	//oglUseProgram(_pipeline_id);
	//oglUseProgram(_vert_shader_id);
	//oglUseProgram(_frag_shader_id);

	{
		char err[256] = "";
		gl_error_string(err);
		if (strlen(err) != 0) {
			OUTPUT_ERROR("OpenGL error : %s", err);
			return;
		}
	}

	/* VAO */
	//oglGenVertexArrays(1, &_vertex_array_id);
	oglBindVertexArray(_vertex_array_id);

	/* VBO */
	//oglBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);
	//oglBufferData(GL_ARRAY_BUFFER, sizeof(test_tri),
			//test_tri, GL_STATIC_DRAW);

	//oglEnableVertexAttribArray(0);
	//oglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glDrawArrays(GL_TRIANGLES, GL_POINTS, 3);
	//oglDisableVertexAttribArray(0);
	//oglDeleteVertexArrays(1, &_vertex_array_id);
	oglBindVertexArray(0);
	oglBindProgramPipeline(0);
	{
		char err[256] = "";
		gl_error_string(err);
		if (strlen(err) != 0) {
			OUTPUT_ERROR("OpenGL error : %s", err);
			return;
		}
	}

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

