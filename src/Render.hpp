#ifndef __RENDER_H__
#define __RENDER_H__

//  Std lib
#include <memory>
#include <unordered_map>
using std::unordered_map;
#include <vector>
using std::vector;

//  Libs
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// 3rd Party
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//  Engine lib
#include "Model.hpp"
#include "Shader.hpp"
#include "Actor.hpp"
#include "Camera.hpp"
#include "Display.hpp"

class Renderer {
private:
    ModelID GID;
    ShaderID SID;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Display> display;
public:
    unordered_map<ModelID, std::shared_ptr<Model>> models;
    unordered_map<ShaderID, std::shared_ptr<ShaderProgram>> shaders;

    void set_display( std::shared_ptr<Display> disp );
    void set_camera( std::shared_ptr<Camera> camera );

    Renderer( ):GID(0),SID(0) {};
    ~Renderer();
    void                init( );
    void                render(std::vector<std::shared_ptr<Actor> > actors );
    ModelID add_model( std::shared_ptr<Model> model );
    std::shared_ptr<Model>   get_model (ModelID mid);
    ShaderID add_shader(std::shared_ptr<ShaderProgram> shader);
    std::shared_ptr<ShaderProgram> get_shader(ModelID mid);
};

#endif


/**
 * @@TODO:
 * Screen Renderer
 * Texture Renderer
 * Movie Renderer
 * OpenCL Renderer
 *
 * Otherwise need to make this class generic.
 **/
