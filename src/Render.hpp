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

class Renderer {
private:
    ModelID GID;
public:
    unordered_map<ModelID, std::shared_ptr<Model>> models;
    unordered_map<ShaderID, std::shared_ptr<Shader>> shaders;

    Renderer( ):GID(0) {};
    ~Renderer();
    void                init( );
    void                Render(std::vector<std::shared_ptr<Actor> > &actors );
    void                add_model( std::shared_ptr<Model> model );
    std::shared_ptr<Model>   get_model (ModelID mid);
};

#endif
