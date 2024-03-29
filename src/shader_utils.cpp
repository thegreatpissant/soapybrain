/**
 * As per:
 * http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_02
 */

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>

typedef struct shaderinfo {
    GLuint shadertype;
    const char *filename;
} ShaderInfo;

std::string file_read_string( const char *filename ) {
    std::fstream ifile;
    ifile.open( filename, std::ios_base::in );

    std::string istring, estring;
    while ( getline( ifile, istring ) ) {
        estring += istring + '\n';
    }
    ifile.close( );
    return estring;
}

/**
 * Display compilation errors from the OpenGL shader compiler
*/
std::string get_shader_log( GLuint object ) {
    GLint log_length = 0;
    if ( glIsShader( object ) )
        glGetShaderiv( object, GL_INFO_LOG_LENGTH, &log_length );
    else if ( glIsProgram( object ) )
        glGetProgramiv( object, GL_INFO_LOG_LENGTH, &log_length );
    else {
        return std::string("PRINTLOG: Not a Shader or a Program");
    }

    char *log = new char[log_length];
    if ( glIsShader( object ) )
        glGetShaderInfoLog( object, log_length, NULL, log );
    else if ( glIsProgram( object ) )
        glGetProgramInfoLog( object, log_length, NULL, log );
    std::string message = log;
    delete[] log;
    return message;
}

/**
 * Compile the shader from 'filename', with error handeling
 */
GLuint create_shader_string( const char *filename, GLenum type ) {
    std::string shader_source = file_read_string( filename );
    if ( shader_source.empty( ) ) {
        std::cerr << "Error Opening " << filename << std::endl;
        exit( EXIT_FAILURE );
    }
    GLuint res = glCreateShader( type );
    std::string source;
    source += shader_source;
    const GLchar *c_source = source.c_str( );
    glShaderSource( res, 1, &c_source, NULL );
    glCompileShader( res );
    GLint compile_ok = GL_FALSE;
    glGetShaderiv( res, GL_COMPILE_STATUS, &compile_ok );
    if ( compile_ok == GL_FALSE ) {
        std::cerr << "Compile failed on file : " << filename << std::endl;
        std::cerr << get_shader_log( res ) << std::endl;
        glDeleteShader( res );
        exit( EXIT_FAILURE );
    }
    return res;
}

GLuint LoadShaders( ShaderInfo *si ) {
    GLuint vertshader = create_shader_string( si->filename, si->shadertype );
    si++;
    GLuint fragshader = create_shader_string( si->filename, si->shadertype );
    GLuint program = glCreateProgram( );
    glAttachShader( program, vertshader );
    glAttachShader( program, fragshader );
    glLinkProgram( program );
    GLint link_ok;
    glGetProgramiv( program, GL_LINK_STATUS, &link_ok );
    if ( !link_ok ) {
        std::cerr << "glLinkProgram: " << get_shader_log ( program ) << std::endl;
        return 0;
    }
    return program;
}




#ifdef false
void asdf () {

#ifdef OPENGL_ES
    source += "#version 100\n";
#else
    source += "#version 120\n";
#endif
    //  GLES2 precision specifiers
#ifdef GL_ES_VERSION_2_0
    /*  Define default float precision for fragment shaders: */
    if ( type == GL_FRAGMENT_SHADER ) {
        source += "#ifdef GL_FRAGMENT_PRECISION_HIGH\n";
        source += "precision highp float;           \n";
        source += "#else                            \n";
        source += "precision mediump float;         \n";
        source += "#endif                           \n";
    }
    /*  Note: OpengGL ES automatically defines this:
    #define GL_ES
*/
#else
    /* Ignore GLES 2 precision specifiers: */
    source += "#define lowp    \n";
    source += "#define mediump \n";
    source += "#define highp   \n";
#endif

}
#endif
