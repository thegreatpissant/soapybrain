#ifndef SHADER_H_
#define SHADER_H_

#include <glm/glm.hpp>

#include <stdexcept>
#include <string>
#include <map>
#include <vector>

#include <GL/glew.h>

using ShaderID = GLint;

typedef struct shaderinfo
{
    GLuint shadertype;
    const char *filename;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo *si);
std::string shader_log (GLuint object);
//  ------------------------------------------------------- //
//  The following shader class is inspired from the
//  OpenGL 4.0 Shading Language Cookbook, David Wolff

class ShaderProgramException : public std::runtime_error
{
  public:
    ShaderProgramException(const std::string &msg) : std::runtime_error(msg)
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
    std::string source;
    bool compiled;

  public:
    Shader( );
    Shader(GLenum type);
    ~Shader( );

    void Compile( ); 
    void Type(GLenum ntype);
    void Source(std::string shader_source);
    void SourceFile(std::string filename);
    void Delete( );

    std::string Dump ();

    GLenum GetType( ) const;
    GLuint GetObject( ) const;
    GLuint GetHandle() const;
    std::string GetSource( ) const;
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
    std::vector<GLuint> shader_handles;
    GLint uniform_count;
    std::map<std::string, int> uniformLocations;
    std::string name;

    int getUniformLocation(const char * name);

  public:
    ShaderProgram( );
    ~ShaderProgram( );

    void validate( );

    bool isLinked( );

    void bindAttributeLocation(GLuint location, const char *name);
    void bindFragDataLocation(GLuint location, const char *name);

    void addShader(GLuint shader_handle);
    void addShader(Shader &shader);
    void compileShader(const char *filename, ShaderType shader_type);
    void link( );
    void setUniform(const char *name, const glm::vec2 &v);
    void setUniform(const char *name, const glm::vec3 &v);
    void setUniform(const char *name, const glm::vec4 &v);
    void setUniform(const char *name, const glm::mat4 &m);
    void setUniform(const char *name, const glm::mat3 &m);
    void setUniform(const char *name, float val);
    void setUniform(const char *name, int val);
    void setUniform(const char *name, bool val);
    void use( );
    void unuse ();
    void cleanup ( );

    void setName(std::string name);
    std::string getName();

    int getHandle( );
    void scrape( );
    void scrape_uniforms( );
    void printActiveUniforms( );
    void printActiveAttributes( );
    void printActiveUniformBlocks( );
};
#endif
