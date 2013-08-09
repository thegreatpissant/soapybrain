#ifndef __RENDER_H__
#define __RENDER_H__

//  Std lib
#include <vector>

//  Engine lib
#include "Model.h"

class render {
 public:
  std::vector <model_t*> models;
  render () { 
  };
  void Render () {
    for (auto a : models ) {
      a->Render ();
    }
  }
};

#endif
