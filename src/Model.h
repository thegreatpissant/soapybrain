#ifndef __MODEL_H__
#define __MODEL_H__

#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cerr;
using std::endl;

using ModelID = GLint;

#include "Actor.h"

enum Attrib_IDs
{
    vPosition = 0
};
#define BUFFER_OFFSET( offset ) ( ( void * )( offset ) )

class Model
{
  private:

  public:
    long numVertices;
    string name;
    vector<float> vertices;
    vector<GLuint> vaos;
    vector<GLuint> buffers;
    GLint renderPrimitive;
    Model( )
    {
    }
    virtual void render( ) const = 0;
};

class Simple_equation_model_t : public Model
{
  public:
    void render( ) const
    {

        //  What Shaders do we use for this model?
        //  Based on the entity state.

        //  Bind to that shader setup and program

        glBindVertexArray( vaos[0] );
        glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
        glDrawArrays( renderPrimitive, 0, numVertices );

        //  Unbind from that shader
    }
    void setup_render_model( )
    {
        this->vaos.resize( 1 );
        glGenVertexArrays( this->vaos.size( ), &this->vaos[0] );
        if ( this->vaos[0] == 0 )
        {
            cerr << this->name << " - Did not get a valid Vertex Attribute Object" << endl;
        }
        glBindVertexArray( this->vaos[0] );

        //  Buffer Data
        this->buffers.resize( 1 );
        glGenBuffers( this->buffers.size( ), &this->buffers[0] );
        glBindBuffer( GL_ARRAY_BUFFER, this->buffers[0] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * this->vertices.size( ), &this->vertices[0],
                      GL_STATIC_DRAW );
        glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET( 0 ) );
        glEnableVertexAttribArray( vPosition );
        glBindVertexArray( 0 );
    }
};
#endif  //  __MODEL_H__
