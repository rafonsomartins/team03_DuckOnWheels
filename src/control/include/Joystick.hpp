#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <linux/joystick.h>

class Joystick {
private:
    int fd;

public:
    Joystick(const char* device = "/dev/input/js0");
    ~Joystick();
    
    bool read_event(struct js_event* event);
    
    static float map_axis(int16_t value);
    static float map_axis_to_angle(int16_t value);
    static float apply_deadzone(float value, float deadzone = 0.15f);
};

#endif