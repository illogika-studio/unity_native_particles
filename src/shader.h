#define CODE(...) #__VA_ARGS__

const static char* vert_shader_src = CODE(
	#version 450 core\n
	layout (location = 0) in vec3 vert_position;
	layout (location = 1) in vec4 vert_color;

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
			cos_b*cos_g, cos_g*sin_a*sin_b - cos_a*sin_g, cos_a*cos_g*sin_b + sin_a*sin_g, 0.f,
			cos_b*sin_g, cos_a*cos_g + sin_a*sin_b*sin_g, -cos_g*sin_a + cos_a*sin_b*sin_g, 0.f,
			-sin_b, cos_b*sin_a, cos_a*cos_b, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
		mat4 s = mat4(
			transform_mat[2][0], 0.f, 0.f, 0.f,
			0.f, transform_mat[2][1], 0.f, 0.f,
			0.f, 0.f, transform_mat[2][2], 0.f,
			0.f, 0.f, 0.f, 1.f
		);
		return t * s;
	}

	void main() {
		//gl_Position = (proj_mat * view_mat * model_mat) * vec4(vert_position, 1.f);
		//vec4 p = vec4(vert_position.x + sin(time), vert_position.yz, 1.f);
		vec4 p = vec4(vert_position, 1.f);
		gl_Position = (proj_mat * view_mat * model_mat * calc_initial_position()) * p;
		//gl_Position = vec4(vert_position, 1.f);
	}\0
);

const static char* frag_shader_src = CODE(
	#version 450 core\n
	out vec4 frag_color;

	void main() {
		frag_color = vec4(0.86, 0.62, 0.86, 0.8f);
	}\0
);

