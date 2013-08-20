#ifndef __RENDER_H__
#define __RENDER_H__

//  Std lib
#include <unordered_map>

//  Engine lib
#include "Model.h"
#include "Actor.h"

class Renderer {
 public:
  int current_model_id;
  std::unordered_map <ModelID, shared_ptr<Model>> models;
  Renderer ():current_model_id{0} { 
  };
  void render ( Actor a ) {
    models[a.model_id]->render(a.state);
  }
};

#endif
