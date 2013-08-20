#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Entity.h"

class Camera : public Entity {
 public:
  Camera () {};

  Camera (float px, float py, float pz, float ox, float oy, float oz) 
    {
      ::Entity(px, py, pz, ox, oy, oz);
    };

};
#endif
