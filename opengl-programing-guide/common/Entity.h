/*
 * Todo:
 * - Use Quaterion orientation
 * - Make a pure base class.  Do we use virtual somewhere?
 */

#ifndef __ENTITY_H__
#define __ENTITY_H__

struct EntityState {
  float position_x, position_y, position_z;
  float orientation_x, orientation_y, orientation_z; 

  EntityState (float px, float py, float pz, float ox, float oy, float oz):position_x(px), position_y{py}, position_z{pz}, orientation_x{ox}, orientation_y{oy}, orientation_z{oz}  {
  }

  EntityState () :position_x(0.0f), position_y{0.0f}, position_z{0.0f}, orientation_x{0.0f}, orientation_y{0.0f}, orientation_z{0.0f}
  {}
};

class Entity {
 public:
  EntityState state;

  Entity ( float px, float py, float pz, float ox, float oy, float oz)
  {
    this->state.position_x = px;
    this->state.position_y = py;
    this->state.position_z = pz;
    this->state.orientation_x = ox;
    this->state.orientation_y = oy;
    this->state.orientation_z = oz;
  }
  Entity ()
  {}

};

#endif
