#define CODE(...) #__VA_ARGS__

const static char* vert_shader_src = CODE(
	#version 450 core\n
	layout (location = 0) in vec3 position;

	out gl_PerVertex {
		vec4 gl_Position;
	};

	void main() {
		gl_Position = vec4(position.x, position.y, position.z, 1.0);
	}\0
);

const static char* frag_shader_src = CODE(
	#version 450 core\n
	out vec4 color;

	void main() {
		color = vec4(1.0, 0.5, 0.2, 1.0);
	}\0;
);

