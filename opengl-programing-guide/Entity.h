/*
 * Todo:
 * - Use Quaterion orientation
 * - Make a pure base class.  Do we use virtual somewhere?
 */

#ifndef __ENTITY_H__
#define __ENTITY_H__

class entity {
 public:
  float _px, _py, _pz;
  float _ox, _oy, _oz; 
  int model_id;
  entity ( float px, float py, float pz, float ox, float oy, float oz)
    :_px{px}, _py{py}, _pz{pz}, _ox{ox}, _oy{oy}, _oz{oz} 
  {}
  entity () :_px{0.0f}, _py{0.0f}, _pz{0.0f}, _ox{0.0f}, _oy{0.0f}, _oz{0.0f} 
  {}

};

#endif
