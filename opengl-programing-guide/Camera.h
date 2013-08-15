#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Entity.h"

class camera : public entity {
 public:
  camera () {};

  camera (float px, float py, float pz, float ox, float oy, float oz) 
    {
      ::entity(px, py, pz, ox, oy, oz);
    };

};
#endif
