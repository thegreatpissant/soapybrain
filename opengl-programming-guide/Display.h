#ifndef __DISPLAY_H__
#define __DISPLAY_H__

class Display {
 public:
  int screenWidth;
  int screenHeight;
  float hScreenSize;
  float vScreenSize;
  
  
  Display (): screenWidth {640}, screenHeight{480} { }
};

#endif
