#ifndef ROBOTCONTROLLER_HPP
#define ROBOTCONTROLLER_HPP

#include "DualPCA9685.hpp"
#include "Joystick.hpp"

class RobotController {
private:
    DualPCA9685 robot;
    Joystick joystick;
    
    float throttle;
    float steering_angle;
    float throttle_gain;
    float deadzone;

public:
    RobotController();
    
    void run();  // Main control loop
    void process_axis_event(uint8_t axis, int16_t value);
    void process_button_event(uint8_t button, bool pressed);
    void emergency_stop();
};

#endif