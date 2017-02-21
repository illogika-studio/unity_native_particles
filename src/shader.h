#include "globals.h"

#define CODE(...) #__VA_ARGS__

/* Uniforms */
struct uniform_info {
	const char* name;
	GLint id;
};

#define	u_time		0 //uniforms[0]
#define	u_model		1 //uniforms[1]
#define	u_view		2 //uniforms[2]
#define	u_proj		3 //uniforms[3]
#define	u_transform	4 //uniforms[4]

static uniform_info uniforms[] = {
	  { "time", -1 }
	, { "model_mat", -1 }
	, { "view_mat", -1 }
	, { "proj_mat", -1 }
	, { "transform_mat", -1 }
};
static const size_t uniform_num = sizeof(uniforms) / sizeof(uniform_info);

/* Locations */
#define	loc_vert_pos			0
#define	loc_vert_color			1
#define	loc_transform_pos		2
#define	loc_transform_rot		3
#define	loc_transform_scale		4

static void gl_init_uniforms(const uniform_info arr[], const size_t s, const GLuint pipeline_id) {
	for (int i = 0; i < s; ++i) {
		uniforms[i].id = oglGetUniformLocation(pipeline_id, uniforms[i].name);
		if (uniforms[i].id == -1) {
			OUTPUT_ERROR("Couldnt get uniform location : %s", uniforms[i].name);
		}
	}
}

static const char* vert_shader_src = CODE(
	#version 450 core\n
	layout (location = 0)	in vec3 vert_position;
	layout (location = 1)	in vec4 vert_color;
	layout (location = 2)	in vec3 transform_pos;
	layout (location = 3)	in vec3 transform_rot;
	layout (location = 4)	in vec3 transform_scale;

	uniform float time;
	uniform mat4 model_mat;
	uniform mat4 view_mat;
	uniform mat4 proj_mat;
	uniform mat3 transform_mat;

	out gl_PerVertex {
		vec4 gl_Position;
	};

	mat4 calc_initial_position() {
		float cos_a = cos(transform_mat[1][0]);
		float cos_b = cos(transform_mat[1][1]);
		float cos_g = cos(transform_mat[1][2]);
		float sin_a = sin(transform_mat[1][0]);
		float sin_b = sin(transform_mat[1][1]);
		float sin_g = sin(transform_mat[1][2]);

		mat4 t = mat4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			transform_mat[0][0], transform_mat[0][1], transform_mat[0][2], 1.f
		);
		mat4 r = mat4(
			cos_b*cos_g, cos_b*sin_g, -sin_b, 0.f, 
			cos_g*sin_a*sin_b - cos_a*sin_g, cos_a*cos_g + sin_a*sin_b*sin_g, cos_b*sin_a, 0.f, 
			cos_a*cos_g*sin_b + sin_a*sin_g, -cos_g*sin_a + cos_a*sin_b*sin_g, cos_a*cos_b, 0.f, 
			0.f, 0.f, 0.f, 1.f
		);
		mat4 s = mat4(
			transform_mat[2][0], 0.f, 0.f, 0.f,
			0.f, transform_mat[2][1], 0.f, 0.f,
			0.f, 0.f, transform_mat[2][2], 0.f,
			0.f, 0.f, 0.f, 1.f
		);
		return t * r * s;
	}

	void main() {
		//gl_Position = (proj_mat * view_mat * model_mat) * vec4(vert_position, 1.f);
		//vec4 p = vec4(vert_position.x + sin(time), vert_position.yz, 1.f);
		vec4 p = vec4(vert_position, 1.f);
		gl_Position = (proj_mat * view_mat * model_mat * calc_initial_position()) * p;
		//gl_Position = vec4(vert_position, 1.f);
	}\0
);

static const char* frag_shader_src = CODE(
	#version 450 core\n
	out vec4 frag_color;

	void main() {
		frag_color = vec4(0.86, 0.62, 0.86, 0.5f);
	}\0
);

