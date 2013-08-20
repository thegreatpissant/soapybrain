/*
 * Todo:
 * - Use Quaterion orientation
 * - Make a pure base class.  Do we use virtual somewhere?
 */

#ifndef __ENTITY_H__
#define __ENTITY_H__

class entity {
 public:
  float position_x, position_y, position_z;
  float orientation_x, orientation_y, orientation_z; 
  int model_id;
  entity ( float px, float py, float pz, float ox, float oy, float oz)
    :position_x{px}, position_y{py}, position_z{pz}, orientation_x{ox}, orientation_y{oy}, orientation_z{oz} 
  {}
  entity () :position_x{0.0f}, position_y{0.0f}, position_z{0.0f}, orientation_x{0.0f}, orientation_y{0.0f}, orientation_z{0.0f} 
  {}

};

#endif
