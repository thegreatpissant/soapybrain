#ifndef __DISPLAY_H__
#define __DISPLAY_H__

class Display {
 public:
  int screen_width;
  int screen_height;
  float h_screen_size;
  float v_screen_size;
  
  
  Display (): screen_width {640}, screen_height{480} { }
};

#endif
