#ifndef __MODEL_H__
#define __MODEL_H__

#include <GL/glew.h>
#include <vector>

enum Attrib_IDs { vPosition = 0 };
#define BUFFER_OFFSET(offset)  ((void *)(offset))
GLint GID = 0;

class model_t {
 public:
  long numVertices;
  GLint id;
  std::string name;
  std::vector <float> vertices;
  std::vector <GLuint> vaos;
  std::vector <GLuint> buffers;
  int    renderPrimitive;
  model_t () {
    id = GID++;
  }
  virtual void Render () {
  }
};
class simple_equation_model_t: public model_t {
 public:
  void Render () {
    glBindVertexArray (vaos[0]);
    glBindBuffer ( GL_ARRAY_BUFFER, buffers[0]);
    glDrawArrays ( renderPrimitive, 0, numVertices);
  }
  void SetupRenderModel () {
    this->vaos.resize(1);
    glGenVertexArrays( this->vaos.size(), &this->vaos[0] );
    if ( this->vaos[0] == 0 ) {
      std::cerr << this->name << "id: " << this->id << " - Did not get a valid Vertex Attribute Object" << std::endl;
    } 
    glBindVertexArray( this->vaos[0] );
    this->buffers.resize(1);
    glGenBuffers(this->buffers.size(), &this->buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
    glBindVertexArray (0);
  }
};
#endif //  __MODEL_H__
