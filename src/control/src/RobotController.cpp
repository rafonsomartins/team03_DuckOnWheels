#include "../include/RobotController.hpp"
#include <cstdio>
#include <unistd.h>

RobotController::RobotController() 
    : throttle(0.0f), 
      steering_angle(90.0f),
      throttle_gain(0.8f),
      deadzone(0.15f) {
    
    printf("\n=== JetRacer PS3 Controller ===\n");
    printf("Axis 0 (Left stick X): Steering (left/right)\n");
    printf("Axis 1 (Left stick Y): Throttle (forward/backward)\n");
    printf("X button (Button 0): Emergency stop\n");
    printf("Press Ctrl+C to quit\n\n");
    
    robot.set_motor(throttle);
}

void RobotController::process_axis_event(uint8_t axis, int16_t value) {
    if (axis == 0) {
        float raw = Joystick::map_axis(value);
        float steering_input = Joystick::apply_deadzone(raw, deadzone);
        steering_angle = 90.0f + steering_input * 90.0f;
        robot.set_servo_angle(steering_angle);
        printf("Steering: %.1f°", steering_angle);
    } 
    else if (axis == 1) {
        float raw = -Joystick::map_axis(value);
        throttle = Joystick::apply_deadzone(raw, deadzone) * throttle_gain;
        robot.set_motor(-1 * throttle);
        printf("Throttle: %+.2f", throttle);
    }
}

void RobotController::process_button_event(uint8_t button, bool pressed) {
    if (button == 0 && pressed) {
        printf(">>> EMERGENCY STOP! <<<\n");
        emergency_stop();
    }
}

void RobotController::emergency_stop() {
    throttle = 0.0f;
    steering_angle = 90.0f;
    robot.stop_all();
}

void RobotController::run() {
    struct js_event event;
    
    while (true) {
        if (joystick.read_event(&event)) {
            if (event.type & JS_EVENT_INIT) continue;
            
            if (event.type == JS_EVENT_AXIS) {
                printf("Axis %d: %6d | ", event.number, event.value);
                process_axis_event(event.number, event.value);
                printf("\n");
            }
            else if (event.type == JS_EVENT_BUTTON) {
                printf("Button %d: %s\n", event.number, event.value ? "PRESSED" : "released");
                process_button_event(event.number, event.value);
            }
        }
        
        usleep(10000);
    }
}