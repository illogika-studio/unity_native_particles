#define CODE(...) #__VA_ARGS__

const static char* vert_shader_src = CODE(
	#version 430 core\n
	layout (location = 0) in vec3 pos;

	uniform mat4 modelmat;
	uniform mat4 viewmat;
	uniform mat4 projmat;

	out gl_PerVertex {
		vec4 gl_Position;
	};

	void main() {
		vec4 p = vec4(10.f, 10.f, 10.f, 1.f);
		gl_Position = projmat * viewmat * modelmat * p;
	}\0
);

const static char* frag_shader_src = CODE(
	#version 430 core\n
	out vec4 color;

	void main() {
		color = vec4(0.86, 0.62, 0.86, 1.0);
	}\0
);

