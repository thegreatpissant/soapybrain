#ifndef __DISPLAY_H__
#define __DISPLAY_H__

class Display {
private:
    int screen_width;
    int screen_height;
    float h_screen_size;
    float v_screen_size;

  public:
    Display( ) : screen_width{ 640 }, screen_height{ 480 } {}
    void Reshape ( int newWidth, int newHeight);
    int getWidth () const;
    int getHeight () const;
};

#endif
