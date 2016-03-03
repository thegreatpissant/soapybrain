#ifndef SHADER_H_
#define SHADER_H_

#include <glm/glm.hpp>

#include <stdexcept>
#include <string>
#include <map>
#include <vector>
using std::string;
using std::map;
using std::vector;

#include <GL/glew.h>

using ShaderID = GLint;

typedef struct shaderinfo
{
    GLuint shadertype;
    const char *filename;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo *si);
string shader_log (GLuint object);
//  ------------------------------------------------------- //
//  The following shader class is inspired from the
//  OpenGL 4.0 Shading Language Cookbook, David Wolff

class ShaderProgramException : public std::runtime_error
{
  public:
    ShaderProgramException(const string &msg) : std::runtime_error(msg)
    {
    }
};

enum class ShaderType
{
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETY = GL_GEOMETRY_SHADER,
    TESS_CONTROL = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
    COMPUTE = GL_COMPUTE_SHADER
};

class Shader
{
  private:
    GLenum type;
    GLuint object;
    string source;
    bool compiled;

  public:
    Shader( );
    Shader(GLenum type);
    ~Shader( );

    void Compile( ) throw(ShaderProgramException);
    void Type(GLenum ntype) throw(ShaderProgramException);
    void Source(string shader_source);
    void SourceFile(string filename)throw(ShaderProgramException);
    void Delete( );

    string Dump ();

    GLenum GetType( ) const;
    GLuint GetObject( ) const;
    GLuint GetHandle() const;
    string GetSource( ) const;
};

// using ShaderID = GLint;

// class Shader {};
//  We need to understand this more before moving forward
//  Different levels of shaders
//  Guarantee that some will exist - ex: Camera and Viewport transforms
//  Some are more model specific - ex: model space transforms
//
//  Vertex -> Model Transform -> Camera Transform -> ViewportTransform
//
//  How do we chain these properly?

class ShaderProgram
{
  private:
    GLuint handle;
    bool linked;
    vector<GLuint> shader_handles;
    GLint uniform_count;
    map<string, int> uniformLocations;

    int getUniformLocation(const char *name);

  public:
    ShaderProgram( );
    ~ShaderProgram( );

    void validate( ) throw(ShaderProgramException);

    bool isLinked( );

    void bindAttributeLocation(GLuint location, const char *name);
    void bindFragDataLocation(GLuint location, const char *name);

    void addShader(GLuint shader_handle);
    void addShader(Shader &shader);
    void compileShader(const char *filename, ShaderType shader_type) throw(ShaderProgramException);
    void link( ) throw(ShaderProgramException);
    void setUniform(const char *name, float x, float y);
    void setUniform(const char *name, const glm::vec3 &v);
    void setUniform(const char *name, const glm::vec4 &v);
    void setUniform(const char *name, const glm::mat4 &m);
    void setUniform(const char *name, const glm::mat3 &m);
    void setUniform(const char *name, float val);
    void setUniform(const char *name, int val);
    void setUniform(const char *name, bool val);
    void use( ) throw(ShaderProgramException);
    void unuse ();
    void cleanup ( );

    int getHandle( );
    void scrape( );
    void scrape_uniforms( );
    void printActiveUniforms( );
    void printActiveAttributes( );
    void printActiveUniformBlocks( );
};
#endif
