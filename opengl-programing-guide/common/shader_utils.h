#ifndef __SHADER_UTILS_H__
#define __SHADER_UTILS_H__
#include <GL/glew.h>
#include <GL/glut.h>

typedef  struct shaderinfo {
  GLuint shadertype;
  const char * filename;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo * si);

#endif
