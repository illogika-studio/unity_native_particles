#define CODE(...) #__VA_ARGS__

const static char* vert_shader_src = CODE(
	#version 450 core\n
	layout (location = 0) in vec3 vert_position;
	layout (location = 1) in vec4 vert_color;

	uniform mat4 model_mat;
	uniform mat4 view_mat;
	uniform mat4 proj_mat;

	out gl_PerVertex {
		vec4 gl_Position;
	};

	void main() {
		vec4 p = vec4(2.f, 1.f, -1.f, 1.f);
		gl_Position = proj_mat * view_mat * model_mat * p;
	}\0
);

const static char* frag_shader_src = CODE(
	#version 450 core\n
	out vec4 frag_color;

	void main() {
		frag_color = vec4(0.86, 0.62, 0.86, 1.0);
	}\0
);

