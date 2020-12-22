#include "Shader.hpp"
#include <cstdlib>

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <fstream>
using std::fstream;

#include <sstream>
#include <string>
using std::string;
#include <cstring>

#include <algorithm>
using std::for_each;
/*
 * Shader Class
 */
Shader::Shader (): type (GL_ZERO), object (0), compiled (false)  {}
Shader::Shader (GLenum ntype): type (ntype), object (0), compiled (false) {}

Shader::~Shader ()
{
    if (compiled)
        Delete();
}

void Shader::Compile ()
throw (ShaderProgramException)
{
    if ( type == GL_ZERO ) {
        string msg = string ("Shader type not specified.");
        throw (ShaderProgramException(msg));
    }

    if ( source.empty() ) {
        string msg = string ("Shader souce empty.");
        throw (ShaderProgramException(msg));
    }

    object  = glCreateShader ( type );
    if ( object == 0 ) {
        string msg = string ("Failed to get Shader Object.");
        throw (ShaderProgramException(msg));
    }

    //  Associate source
    const GLchar * c_source = source.c_str ();
    glShaderSource ( object, 1, &c_source, NULL);

    //  Compile and check
    glCompileShader ( object );
    GLint compile_ok = GL_FALSE;
    glGetShaderiv (object, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE ) {
        string msg = string("Failed to compile shader.") + this->source + " " + shader_log(object);
        glDeleteShader (object);
        throw (ShaderProgramException(msg));
    }
    compiled = true;
}

void Shader::Source( string shader_source )
{
    compiled = false;
    source = shader_source;
}

void Shader::SourceFile(string filename)
throw (ShaderProgramException)
{
    compiled = false;
    fstream ifile;
    ifile.open( filename.c_str(), std::ios_base::in );
    if (!ifile.is_open()) {
        string msg = string ("Failed to open file: ") + filename;
        throw (ShaderProgramException (msg));
    }

    string istring, estring;
    while ( getline( ifile, istring ) ) {
        estring += istring + '\n';
    }
    ifile.close( );
    Source(estring);
}

GLuint Shader::GetObject() const
{
    return object;
}

GLuint Shader::GetHandle() const
{
    return object;
}

string Shader::GetSource() const {
    return source;
}

GLenum Shader::GetType() const
{
    return type;
}

void Shader::Delete()
{
    glDeleteShader (this->object);
}

std::string Shader::Dump()
{
    string info;
    info += "Shader dump log\n";
    std::stringstream ss;
    ss << "Shader object id: " << object << "\n";
    info += ss.str();
    if (glIsShader(object))
        info += "This is a valid shader\n";
    else
        info += "This is not a valid shader\n";
    info += "Shader type: ";
    switch (this->type) {
    case GL_VERTEX_SHADER: info += "vertex shader";
        break;
    case GL_FRAGMENT_SHADER: info += "fragment shader";
        break;
    default: info += "Unknown type";
        break;
    }
    return info;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
string shader_log( GLuint object ) {
    GLint log_length = 0;
    if ( glIsShader( object ) )
        glGetShaderiv( object, GL_INFO_LOG_LENGTH, &log_length );
    else if ( glIsProgram( object ) )
        glGetProgramiv( object, GL_INFO_LOG_LENGTH, &log_length );
    else {
        return string("PRINTLOG: Not a Shader or a Program");
    }

    char *log = new char[log_length];
    if ( glIsShader( object ) )
        glGetShaderInfoLog( object, log_length, NULL, log );
    else if ( glIsProgram( object ) )
        glGetProgramInfoLog( object, log_length, NULL, log );
    string message = log;
    delete[] log;
    return message;
}
/*
 * ShaderProgram Class
 */


ShaderProgram::ShaderProgram (): linked(false), uniform_count(0)
{
}
ShaderProgram::~ShaderProgram()
{
    cleanup ();
}

void ShaderProgram::addShader(GLuint shader_handle)
{
    shader_handles.push_back(shader_handle);
}

void ShaderProgram::addShader(Shader &shader)
{
    shader_handles.push_back(shader.GetHandle());
}

void ShaderProgram::link ()
throw (ShaderProgramException)
{
    //  Do we have shaders to link
    if (shader_handles.empty()) {
        string msg = string ("No Shader program handles to link.");
        throw (ShaderProgramException (msg));
    }
    //  Request a shader program
    this->handle = glCreateProgram ();
    if (handle == 0 ) {
        string msg = string("Failed to create Shader Program.");
        throw (ShaderProgramException(msg));
    }
    //  Add all compiled shaders to the program
    for ( int i = 0; i < shader_handles.size(); i++ ) {
        cout << "Shader handle : " << shader_handles[i] << endl;

        if (!glIsShader(shader_handles[i]))
            throw (ShaderProgramException("This is not a shader"));

        glAttachShader ( handle, shader_handles[i] );
        string msg;
        switch (glGetError()) {
        case GL_NO_ERROR:
            break;
	case GL_INVALID_ENUM:
            msg = string ("Enumeration argument not valid");
            break;
        case GL_INVALID_VALUE:
            msg = string ("value not valid");
            break;
        case GL_INVALID_OPERATION:
            msg = string  ("not a shader object, or already in a program");
            break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
            msg = string ("framebuffer object is not complete");
            break;
	case GL_OUT_OF_MEMORY:
	    msg = string ("not enough memory to execute the command");
            break;
        case GL_STACK_UNDERFLOW:
            msg = string ("Command execution would cause internal stack underflow");
            break;
        case GL_STACK_OVERFLOW:
            msg = string ("Command execution would cause internal stack overflow");
            break;
        default:
            msg = string ("Unknown error occured");
            break;
        }
        if (msg.size() > 0)
            throw (ShaderProgramException(msg));
            

    }
    //  Link the shaders
    glLinkProgram ( handle );
    GLint link_ok;
    glGetProgramiv ( handle, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        string msg  = string ("glLinkProgram: ") + shader_log( handle );
        throw (ShaderProgramException(msg));
    }
    this->linked = true;
    scrape();
}

void ShaderProgram::setName(string name)
{
    this->name = name;
}

string ShaderProgram::getName()
{
    return this->name;
}

void ShaderProgram::use ()         throw (ShaderProgramException)
{
    glUseProgram (getHandle());
}

void ShaderProgram::unuse ()
{
    glUseProgram (0);
}

void ShaderProgram::cleanup()
{
    unuse();
    glDeleteProgram(getHandle());
}

void ShaderProgram::validate ()    throw (ShaderProgramException)
{

}

int ShaderProgram::getHandle ()
{
    return this->handle;
}

bool ShaderProgram::isLinked ()
{
    return this->linked;
}

void ShaderProgram::bindAttributeLocation (GLuint location, const char * name)
{

}

void ShaderProgram::bindFragDataLocation  (GLuint location, const char * name)
{

}

void ShaderProgram::setUniform (const char *name, float x, float y)
{
    float v2[2];
    v2[0] = x; v2[1] = y;
    glUniform2fv (uniformLocations.at(string(name)), 1, &v2[0]);
}

void ShaderProgram::setUniform(const char *name, const glm::vec3 & v )
{
    glUniform3fv (uniformLocations.at(string(name)), 1, &v[0]);
}

void ShaderProgram::setUniform(const char * name, const glm::vec4 &v )
{
    glUniform4fv (uniformLocations.at(string(name)), 1, &v[0]);
}

void ShaderProgram::setUniform(const char * name, const glm::mat4 &m )
{
    glUniformMatrix4fv (uniformLocations.at(string(name)), 1, GL_FALSE, &m[0][0]);
}

void ShaderProgram::setUniform(const char * name, const glm::mat3 &m )
{
    glUniformMatrix3fv (uniformLocations.at(string(name)), 1, GL_FALSE, &m[0][0]);
}

void ShaderProgram::setUniform(const char * name, float val)
{
    glUniform1f (uniformLocations.at(string(name)), val);
}

void ShaderProgram::setUniform(const char *name, int val)
{
    glUniform1i (uniformLocations.at(string(name)), val);
}

void ShaderProgram::setUniform(const char *name, bool val)
{

}

void ShaderProgram::printActiveUniforms ()
{
    use();
    cout << "Printing active uniforms for shader program" << endl;
    for_each (uniformLocations.begin(), uniformLocations.end(),
              [] (const map<string, int>::value_type &elem) {
        cout << elem.first << ": " << elem.second << endl;
    });
    cout << "End of uniform list." << endl;
    unuse();
}

void ShaderProgram::printActiveAttributes ()
{

}

void ShaderProgram::printActiveUniformBlocks()
{

}

//  Get all the variables from our linked shader program
void ShaderProgram::scrape()
{
    use();
    scrape_uniforms();
    unuse();
}

//  Get all the active uniforms for the linked shader
void ShaderProgram::scrape_uniforms ()
{
    if (!this->isLinked())
        return;
    //  @@TODO: Should use the newer API's in OpenGL 4.3 and above if available.
    //  See https://www.opengl.org/wiki/Program_Introspection

    //  Query number of active uniforms
    glGetProgramiv(getHandle(), GL_ACTIVE_UNIFORMS, &uniform_count );
    //  Query for the largest char array a name would occupy
    GLint uni_max_length = 0;
    glGetProgramiv(getHandle(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &uni_max_length);
    char * uni_name = new char[uni_max_length];
    //  Get those names and locations
    for (auto i= 0; i < uniform_count; i++)
    {
        memset (uni_name, '\0', uni_max_length);
        GLsizei uni_name_length = 0;
        glGetActiveUniformName( getHandle(), i, uni_max_length, &uni_name_length, uni_name );
        string tmp_uniform_name = string (uni_name); //, uni_name_length);
        uniformLocations.insert(std::make_pair(tmp_uniform_name,i));
    }
    //  Cleanup
    delete[] uni_name;
}
