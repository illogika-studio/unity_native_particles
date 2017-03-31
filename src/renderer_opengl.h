#pragma once
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"


class Renderer {
public:
	Renderer();
	~Renderer();

	void render(float delta_time);

	void mvp(const GLfloat m[16], const GLfloat v[16], const GLfloat p[16]) {
		memcpy(_model_mat, m, sizeof(GLfloat) * 16);
		memcpy(_view_mat, v, sizeof(GLfloat) * 16);
		memcpy(_projection_mat, p, sizeof(GLfloat) * 16);
	}

private:
	void InitVAO();
	void BindAndFillVBOWithVAO();
	//void BindAndFillVBOWithoutVAO(bool enable_attrib_divisor = false);
	//void DrawDataWithUniforms();

	GLuint _pipeline_id = 0;
	GLuint _vert_shader_id = 0;
	GLuint _frag_shader_id = 0;
	GLuint _vertex_array_id = 0;
	GLuint _vbo_vert_pos = 0;
	GLuint _vbo_data = 0;

	GLfloat _model_mat[16] = {};
	GLfloat _view_mat[16] = {};
	GLfloat _projection_mat[16] = {};

};
