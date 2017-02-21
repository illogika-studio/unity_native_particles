#include "renderer_opengl.h"
#include "shader.h"
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>

Renderer::Renderer(GLsizei particle_qty)
	: _data(particle_qty)
{
	if (particle_qty == 0) {
		OUTPUT_ERROR("Please set the quantity of particles desired.\n");
	}
	if (!init_gl_funcs()) {
		OUTPUT_ERROR("Could not get OpenGL functions.\n");
	}
}

Renderer::~Renderer()
{}

void Renderer::init_opengl() {

	OUTPUT_MSG("OpenGL version : %s", glGetString(GL_VERSION));
	//OUTPUT_MSG("OpenGL supported extensions :\n%s", glGetString(GL_EXTENSIONS));
	GL_CHECK_ERROR();

	/* CANT MAKE IT WORK :(
	_vert_shader_id = oglCreateShaderProgramv(GL_VERTEX_SHADER, 1,
			&vert_shader_src);
	_frag_shader_id = oglCreateShaderProgramv(GL_FRAGMENT_SHADER, 1,
			&frag_shader_src);

	DEBUG_GL();

	oglGenProgramPipelines(1, &_pipeline_id);
	oglBindProgramPipeline(_pipeline_id);
	oglUseProgramStages(_pipeline_id, GL_VERTEX_SHADER_BIT, _vert_shader_id);
	oglUseProgramStages(_pipeline_id, GL_FRAGMENT_SHADER_BIT, _frag_shader_id);

	int was_init = 1;
	was_init *= gl_program_was_linked(_vert_shader_id);
	was_init *= gl_program_was_linked(_frag_shader_id);
	was_init *= gl_program_was_linked(_pipeline_id);

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
	*/

	/* Less fun shader compiling. */
	_pipeline_id = oglCreateProgram();
	_frag_shader_id = oglCreateShader(GL_FRAGMENT_SHADER);
	oglShaderSource(_frag_shader_id, 1, &frag_shader_src, nullptr);
	oglCompileShader(_frag_shader_id);

	_vert_shader_id = oglCreateShader(GL_VERTEX_SHADER);
	oglShaderSource(_vert_shader_id, 1, &vert_shader_src, nullptr);
	oglCompileShader(_vert_shader_id);

	oglAttachShader(_pipeline_id, _vert_shader_id);
	oglAttachShader(_pipeline_id, _frag_shader_id);

	// bind the attribute locations (inputs)
	oglBindAttribLocation(_pipeline_id, 0, "vert_position");
	oglBindAttribLocation(_pipeline_id, 1, "vert_color");

	// bind the FragDataLocation (output)
	oglBindFragDataLocation(_pipeline_id, 0, "frag_color");

	oglLinkProgram(_pipeline_id);
	GL_CHECK_ERROR();

	int was_init = 1;
	was_init *= gl_shader_was_compiled(_vert_shader_id);
	was_init *= gl_shader_was_compiled(_frag_shader_id);
	was_init *= gl_program_was_linked(_pipeline_id);
	if (!was_init) {
		OUTPUT_ERROR("Problem initializing OpenGL. Will not render anything.\n");
		return;
	}

	_model_uniform_id = oglGetUniformLocation(_pipeline_id, "model_mat");
	if (_model_uniform_id == -1) OUTPUT_ERROR("Problem getting uniform location.");

	_view_uniform_id = oglGetUniformLocation(_pipeline_id, "view_mat");
	if (_view_uniform_id == -1) OUTPUT_ERROR("Problem getting uniform location.");

	_projection_uniform_id = oglGetUniformLocation(_pipeline_id, "proj_mat");
	if (_projection_uniform_id == -1) OUTPUT_ERROR("Problem getting uniform location.");

	_transform_uniform_id = oglGetUniformLocation(_pipeline_id, "transform_mat");
	if (_transform_uniform_id == -1) OUTPUT_ERROR("Problem getting uniform location.");

	_time_uniform_id = oglGetUniformLocation(_pipeline_id, "time");
	if (_time_uniform_id == -1) OUTPUT_ERROR("Problem getting uniform location.");

	GL_CHECK_ERROR();

	oglGenVertexArrays(1, &_vertex_array_id);
	oglBindVertexArray(_vertex_array_id);

	oglGenBuffers(1, &_vertex_buffer_id);
	oglBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);
	oglBufferData(GL_ARRAY_BUFFER, sizeof(quad),
			quad, GL_STATIC_DRAW);

	GL_CHECK_ERROR();
}

void Renderer::detroy_opengl() {
	oglDeleteVertexArrays(1, &_vertex_array_id);
	oglDeleteBuffers(1, &_vertex_buffer_id);

	oglDetachShader(_pipeline_id, _vert_shader_id);
	oglDetachShader(_pipeline_id, _frag_shader_id);
	oglDeleteShader(_vert_shader_id);
	oglDeleteShader(_frag_shader_id);
	oglDeleteProgram(_pipeline_id);
}

void Renderer::update(float time, float delta_time) {
	float r = 0.f;

	/* Speed */
	for (int i = 0; i < _data.size; ++i) {
		_data.speed[i] += _data.gravity * delta_time * _data.global_speed;
	}

	/* Position */
	for (int i = 0; i < _data.size; ++i) {
		if (_data.pos[i].y > 10.f && _data.speed[i].y > 0.f) {
			_data.speed[i].y = -_data.speed[i].y;
		} else if (_data.pos[i].y < -10.f && _data.speed[i].y < 0.f) {
			_data.speed[i].y = -_data.speed[i].y;
		}
		_data.pos[i] += _data.speed[i] * delta_time;
	}

	/* Rotation */
	for (int i = 0; i < _data.size; ++i) {
		_data.rot[i].x += 0.1f;
		_data.rot[i].y += 0.0001f;
	}

	/* Scale */
	//for (int i = 0; i < _data.size; ++i) {
	//	_data.scale[i].x = r;
	//	_data.scale[i].y = r;
	//}

}

void Renderer::render(float time, float delta_time) {
	/* Basic render state. */
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	oglUseProgram(_pipeline_id);
	oglUniformMatrix4fv(_model_uniform_id, 1, GL_FALSE, _model_mat);
	oglUniformMatrix4fv(_view_uniform_id, 1, GL_FALSE, _view_mat);
	oglUniformMatrix4fv(_projection_uniform_id, 1, GL_FALSE, _projection_mat);
	oglUniform1f(_time_uniform_id, time);

	/* VBO */
	oglBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);
	//oglBufferData(GL_ARRAY_BUFFER, sizeof(quad),
	//		quad, GL_STATIC_DRAW);

	for (int i = 0; i < _data.size; ++i) {
		//_data.print(i);
		GLfloat t[16] = {
			_data.pos[i].x, _data.pos[i].y, _data.pos[i].z
 			, _data.rot[i].x, _data.rot[i].y, _data.rot[i].z
			, _data.scale[i].x, _data.scale[i].y, _data.scale[i].z
		};
		oglUniformMatrix3fv(_transform_uniform_id, 1, GL_FALSE, t);


		oglEnableVertexAttribArray(0);
		oglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glDrawArrays(GL_TRIANGLE_STRIP, GL_POINTS, 4);
		oglDisableVertexAttribArray(0);
	}

	GL_CHECK_ERROR();
}
