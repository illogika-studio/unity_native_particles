#include <windows.h>
#include <GL/gl.h>
#include "glext.h"

#define gl_func( fun )										\
	fun = (PFN##fun##PROC)wglGetProcAddress(#fun);	\
	if(fun == NULL) {														\
		printf("Could not get proc address: %s\n", #fun);			\
		exit(-1);															\
	}

struct mat4 {
	GLfloat x[16];
};

mat4 test = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

const GLfloat vertices[] = {
	-0.5, -0.5, 0.0,
	0.5, -0.5, 0.0,
	0.0, 0.5, 0.0
};

void render(float dt)
{

	//glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
}
