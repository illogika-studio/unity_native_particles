#pragma once
#include "glext.h"
#include <stdio.h>

#define OUTPUT_ERROR(format, ...) printf("%s(%d) : %s() : " format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define NUMFUNCS 22
const static char* gl_func_names[] = {
	"glCreateShaderProgramv"
	, "glGenProgramPipelines"
	, "glBindProgramPipeline"
	, "glUseProgramStages"
	, "glProgramUniform4fv"
	, "glGetProgramiv"
	, "glGetProgramInfoLog"
	, "glGenVertexArrays"
	, "glBindVertexArray"
	, "glGenBuffers"
	, "glBindBuffer"
	, "glBufferData"
	, "glVertexAttribPointer"
	, "glEnableVertexAttribArray"
	, "glUseProgram"
	, "glDisableVertexAttribArray"
	, "glDeleteVertexArrays"
	, "glBindFragDataLocation"
	, "glActiveShaderProgram"
	, "glGetUniformLocation"
	, "glProgramUniformMatrix4fv"
	, "glProgramUniformMatrix3fv"
};

 extern void* gl_funcs[NUMFUNCS];
 #define oglCreateShaderProgramv	((PFNGLCREATESHADERPROGRAMVPROC)gl_funcs[0])
 #define oglGenProgramPipelines		((PFNGLGENPROGRAMPIPELINESPROC)gl_funcs[1])
 #define oglBindProgramPipeline		((PFNGLBINDPROGRAMPIPELINEPROC)gl_funcs[2])
 #define oglUseProgramStages		((PFNGLUSEPROGRAMSTAGESPROC)gl_funcs[3])
 #define oglProgramUniform4fv		((PFNGLPROGRAMUNIFORM4FVPROC)gl_funcs[4])
 #define oglGetProgramiv			((PFNGLGETPROGRAMIVPROC)gl_funcs[5])
 #define oglGetProgramInfoLog		((PFNGLGETPROGRAMINFOLOGPROC)gl_funcs[6])
 #define oglGenVertexArrays			((PFNGLGENVERTEXARRAYSPROC)gl_funcs[7])
 #define oglBindVertexArray			((PFNGLBINDVERTEXARRAYPROC)gl_funcs[8])
 #define oglGenBuffers				((PFNGLGENBUFFERSPROC)gl_funcs[9])
 #define oglBindBuffer				((PFNGLBINDBUFFERPROC)gl_funcs[10])
 #define oglBufferData				((PFNGLBUFFERDATAPROC)gl_funcs[11])
 #define oglVertexAttribPointer		((PFNGLVERTEXATTRIBPOINTERPROC)gl_funcs[12])
 #define oglEnableVertexAttribArray	((PFNGLENABLEVERTEXATTRIBARRAYPROC)gl_funcs[13])
 #define oglUseProgram				((PFNGLUSEPROGRAMPROC)gl_funcs[14])
 #define oglDisableVertexAttribArray ((PFNGLDISABLEVERTEXATTRIBARRAYPROC)gl_funcs[15])
 #define oglDeleteVertexArrays		((PFNGLDELETEVERTEXARRAYSPROC)gl_funcs[16])
 #define oglBindFragDataLocation	((PFNGLBINDFRAGDATALOCATIONPROC)gl_funcs[17])
 #define oglActiveShaderProgram		((PFNGLACTIVESHADERPROGRAMPROC)gl_funcs[18])
 #define oglGetUniformLocation		((PFNGLGETUNIFORMLOCATIONPROC)gl_funcs[19])
 #define oglProgramUniformMatrix4fv	((PFNGLPROGRAMUNIFORMMATRIX4FVPROC)gl_funcs[20])
 #define oglProgramUniformMatrix3fv	((PFNGLPROGRAMUNIFORMMATRIX3FVPROC)gl_funcs[21])




static inline bool gl_has_error() {
	bool ret = false;

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		OUTPUT_ERROR("OpenGL error: %d", err);
		ret = true;
	}
	return ret;
}

