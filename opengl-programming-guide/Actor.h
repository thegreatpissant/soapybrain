#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "Entity.h"

class actor : public entity {
 public:
  actor () 
    {
      ::entity(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    };
  actor (float px, float py, float pz, float ox, float oy, float oz) 
    {
      ::entity(px, py, pz, ox, oy, oz);
    };
};

#endif
