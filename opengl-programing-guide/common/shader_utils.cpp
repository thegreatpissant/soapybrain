/**
 * As per: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_02
 */

#include "shader_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string file_read_string (const char *filename ) {
  std::fstream ifile;
  ifile.open(filename, ios_base::in);

  string istring, estring;
  while (getline (ifile,istring)) {
    estring += istring + '\n';
  }
  ifile.close();
  return estring;
}
static char *file_read (const char *filename)
{
  FILE *in = fopen (filename, "rb");
  if (in == NULL) return NULL;

  int res_size = BUFSIZ;
  char *res = (char *)malloc (res_size);
  int nb_read_total = 0;
  while (!feof (in) && !ferror (in))
    {
      if (nb_read_total + BUFSIZ > res_size) 
	{
	  if (res_size > 10*1024*1024)
	    break;
	  res_size = res_size * 2;
	  res = (char *)realloc (res, res_size);
	}
      char *p_res = res + nb_read_total;
      nb_read_total += fread (p_res, 1, BUFSIZ, in);
    }
  fclose (in);
  res = (char*)realloc (res, nb_read_total + 1);
  res[nb_read_total] = '\0';
  return res;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void print_log (GLuint object)
{
  GLint log_length = 0;
  if (glIsShader (object))
    glGetShaderiv (object, GL_INFO_LOG_LENGTH, &log_length);
  else if (glIsProgram (object))
    glGetProgramiv (object, GL_INFO_LOG_LENGTH, &log_length);
  else {
    cerr << "PRINTLOG: Not a Shader or a Program" << endl;
    return;
  }

  char *log = (char*)malloc (log_length);
  if (glIsShader (object))
    glGetShaderInfoLog (object, log_length, NULL, log);
  else if (glIsProgram (object))
    glGetProgramInfoLog (object, log_length, NULL, log);
  cerr << log << endl;
  free (log);
}

/**
 * Compile the shader from 'filename', with error handeling
 */
GLuint create_shader_string (const char *filename, GLenum type)
{
  // const GLchar *source = file_read (filename);
  // if (source == NULL)
  //   {
  //     fprintf (stderr, "Error Opening %s: ", filename);
  //     perror ("");
  //     exit (EXIT_FAILURE);
  //   }
  string shader_source = file_read_string (filename);
  if (shader_source.empty()) {
    cerr << "Error Opening " << filename << endl;
    exit (EXIT_FAILURE);
  }

  GLuint res = glCreateShader (type);
  string source;
#ifdef OPENGL_ES
  source += "#version 100\n";
#else
  source += "#version 120\n";
#endif
    /* GLES2 precision specifiers */
#ifdef GL_ES_VERSION_2_0
    /*  Define default float precision for fragment shaders: */
    if (type == GL_FRAGMENT_SHADER) {
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

  source += shader_source;
  const GLchar *c_source = source.c_str();
  glShaderSource (res, 1, &c_source, NULL);
  glCompileShader (res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv (res, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE)
    {
      cerr << "Compile failed on file : " << filename << endl;
      print_log (res);
      glDeleteShader (res);
      exit (EXIT_FAILURE);
    }
  return res;
}

GLuint create_shader (const char *filename, GLenum type)
{
  const GLchar *source = file_read (filename);
  if (source == NULL)
    {
      fprintf (stderr, "Error Opening %s: ", filename);
      perror ("");
      exit (EXIT_FAILURE);
    }
  GLuint res = glCreateShader (type);
  const GLchar *sources[3] = {
    #ifdef OPENGL_ES
    "#version 100\n"
    #else
    "#version 120\n"
    #endif
    ,
    /* GLES2 precision specifiers */
    #ifdef GL_ES_VERSION_2_0
    /*  Define default float precision for fragment shaders: */
    (type == GL_FRAGMENT_SHADER) ?
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "precision highp float;           \n"
    "#else                            \n"
    "precision mediump float;         \n"
    "#endif                           \n"
    : ""
    /*  Note: OpengGL ES automatically defines this:
	#define GL_ES
    */
    #else
    /* Ignore GLES 2 precision specifiers: */
    "#define lowp    \n"
    "#define mediump \n"
    "#define highp   \n"
    #endif
    ,
    source };
  glShaderSource (res, 3, sources, NULL);
  free ((void*)source);
  glCompileShader (res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv (res, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE)
    {
      fprintf (stderr, "%s:", filename);
      print_log (res);
      glDeleteShader (res);
      exit (EXIT_FAILURE);
    }
  return res;
}

GLuint LoadShaders(ShaderInfo * si) {
  //  GLuint vertshader = create_shader ( si->filename, si->shadertype );
  GLuint vertshader = create_shader_string ( si->filename, si->shadertype );
  si++;
  //  GLuint fragshader = create_shader ( si->filename, si->shadertype );
  GLuint fragshader = create_shader_string ( si->filename, si->shadertype );
  GLuint program = glCreateProgram ();
  glAttachShader (program, vertshader);
  glAttachShader (program, fragshader);
  glLinkProgram (program);
  GLint link_ok;
  glGetProgramiv (program, GL_LINK_STATUS , &link_ok);
  if (!link_ok) {
    cerr << "glLinkProgram: ";
    print_log (program);
    return 0;
  }
  return program;
}
