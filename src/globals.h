#pragma once
#include "glext.h"

#define OUTPUT_ERROR(...) printf("%s(%d) : %s() : %s\n", __FILE__, __LINE__, __func__, #__VA_ARGS__);

#define NUMFUNCS 14
const static char* gl_func_names[] = {
	"glCreateShaderProgramv"
	,"glGenProgramPipelines"
	,"glBindProgramPipeline"
	,"glUseProgramStages"
	,"glProgramUniform4fv"
	,"glGetProgramiv"
	,"glGetProgramInfoLog"
	,"glGenVertexArrays"
	,"glBindVertexArray"
	,"glGenBuffers"
	,"glBindBuffer"
	,"glBufferData"
	,"glVertexAttribPointer"
	,"glEnableVertexAttribArray"
};

 extern void* gl_funcs[NUMFUNCS];
 #define oglCreateShaderProgramv		((PFNGLCREATESHADERPROGRAMVPROC)gl_funcs[0])
 #define oglGenProgramPipelines		((PFNGLGENPROGRAMPIPELINESPROC)gl_funcs[1])
 #define oglBindProgramPipeline		((PFNGLBINDPROGRAMPIPELINEPROC)gl_funcs[2])
 #define oglUseProgramStages			((PFNGLUSEPROGRAMSTAGESPROC)gl_funcs[3])
 #define oglProgramUniform4fv		((PFNGLPROGRAMUNIFORM4FVPROC)gl_funcs[4])
 #define oglGetProgramiv				((PFNGLGETPROGRAMIVPROC)gl_funcs[5])
 #define oglGetProgramInfoLog		((PFNGLGETPROGRAMINFOLOGPROC)gl_funcs[6])
 #define oglGenVertexArrays			((PFNGLGENVERTEXARRAYSPROC)gl_funcs[7])
 #define oglBindVertexArray			((PFNGLBINDVERTEXARRAYPROC)gl_funcs[8])
 #define oglGenBuffers				((PFNGLGENBUFFERSPROC)gl_funcs[9])
 #define oglBindBuffer				((PFNGLBINDBUFFERPROC)gl_funcs[10])
 #define oglBufferData				((PFNGLBUFFERDATAPROC)gl_funcs[11])
 #define oglVertexAttribPointer		((PFNGLVERTEXATTRIBPOINTERPROC)gl_funcs[12])
 #define oglEnableVertexAttribArray	((PFNGLENABLEVERTEXATTRIBARRAYPROC)gl_funcs[13])


//#define oglCreateShaderProgramv		((PFNGLCREATESHADERPROGRAMVPROC)wglGetProcAddress("glCreateShaderProgramv"))
//#define oglGenProgramPipelines		((PFNGLGENPROGRAMPIPELINESPROC)wglGetProcAddress("glGenProgramPipelines"))
//#define oglBindProgramPipeline		((PFNGLBINDPROGRAMPIPELINEPROC)wglGetProcAddress("glBindProgramPipeline"))
//#define oglUseProgramStages			((PFNGLUSEPROGRAMSTAGESPROC)wglGetProcAddress("glUseProgramStages"))
//#define oglProgramUniform4fv		((PFNGLPROGRAMUNIFORM4FVPROC)wglGetProcAddress("glProgramUniform4fv"))
//#define oglGetProgramiv				((PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv"))
//#define oglGetProgramInfoLog		((PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog"))
//#define oglGenVertexArrays			((PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays"))
//#define oglBindVertexArray			((PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray"))
//#define oglGenBuffers				((PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers"))
//#define oglBindBuffer				((PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer"))
//#define oglBufferData				((PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData"))
//#define oglVertexAttribPointer		((PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer"))
//#define oglEnableVertexAttribArray	((PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray"))

