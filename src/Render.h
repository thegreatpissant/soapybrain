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

class Renderer {
private:
    ModelID GID;
public:
    unordered_map<ModelID, shared_ptr<Model>> models;
    unordered_map<ShaderID, shared_ptr<Shader>> shaders;

    Renderer( ):GID(0) {};
    ~Renderer();
    void                init( );
    void                Render(std::vector<shared_ptr<Actor> > &actors );
    void                add_model( shared_ptr<Model> model );
    shared_ptr<Model>   get_model (ModelID mid);
};

#endif
