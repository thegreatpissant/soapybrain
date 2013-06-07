#ifndef __MODEL_H__
#define __MODEL_H__

#include <GL/glew.h>

struct model_t {
  long numVertices;
  vector <float> vertices;
  vector <GLuint> vaos;
  vector <GLuint> buffers;
  int    renderPrimitive;
  void Render () {
    glBindVertexArray (vaos[0]);
    glBindBuffer ( GL_ARRAY_BUFFER, buffers[0]);
    glDrawArrays ( renderPrimitive, 0, numVertices);
  }
};

#endif //  __MODEL_H__
