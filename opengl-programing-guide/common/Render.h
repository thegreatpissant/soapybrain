#ifndef __RENDER_H__
#define __RENDER_H__

//  Std lib
#include <memory>
using std::shared_ptr;
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
#include "Model.h"
#include "Shader.h"
#include "Actor.h"
#include "Display.h"
#include "Camera.h"

//  Externs <- Remove these
class Renderer {
private:
    ModelID GID;
    shared_ptr<Camera> camera;
    shared_ptr<Display> display;
public:
    unordered_map<ModelID, shared_ptr<Model>> models;
    unordered_map<ShaderID, shared_ptr<Shader>> shaders;

    Renderer( ):GID(0) {};
    ~Renderer() {};
    void                init( );
    void                render( vector<shared_ptr<Actor>> &actors );
    void                add_model( shared_ptr<Model> model );
    shared_ptr<Model>   get_model (ModelID mid);
    shared_ptr<Camera> setCamera(shared_ptr<Camera> new_camera);
    shared_ptr<Display> setDisplay(shared_ptr<Display> new_display);
};

#endif
