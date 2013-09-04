#ifndef __SHADER_UTILS_H__
#define __SHADER_UTILS_H__
#include <GL/glew.h>

typedef  struct shaderinfo {
  GLuint shadertype;
  const char * filename;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo * si);

#endif
