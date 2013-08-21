#ifndef __RENDER_H__
#define __RENDER_H__

//  Std lib
#include <unordered_map>

//  Engine lib
#include "Model.h"
#include "Shader.h"
#include "Actor.h"

class Renderer {
 public:
  std::unordered_map <ModelID, shared_ptr<Model>> models;
  std::unordered_map <ShaderID, shared_ptr<Shader>> shaders;
  Renderer () { 
  };

  void render ( Actor a ) {
    //  Select appropriate shaders for this model
    //  Will Chain standard MVW transforms as well as effects

    models[a.model_id]->render(a.state);

    //  Unload the shader and continue
  }
};

#endif
