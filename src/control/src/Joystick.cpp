#include "../include/Joystick.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>
#include <stdexcept>
#include <cstdio>
#include <cmath>

Joystick::Joystick(const char* device) {
    fd = open(device, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        throw std::runtime_error("Failed to open joystick. Make sure controller is connected!");
    }
    
    char name[128];
    if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0) {
        strcpy(name, "Unknown");
    }
    printf("Joystick connected: %s\n", name);
}

Joystick::~Joystick() {
    if (fd >= 0) close(fd);
}

bool Joystick::read_event(struct js_event* event) {
    int bytes = read(fd, event, sizeof(*event));
    if (bytes == sizeof(*event)) {
        return true;
    }
    return false;
}

float Joystick::map_axis(int16_t value) {
    return value / 32767.0f;
}

float Joystick::map_axis_to_angle(int16_t value) {
    return 90.0f + (value / 32767.0f) * 90.0f;
}

float Joystick::apply_deadzone(float value, float deadzone) {
    if (fabs(value) < deadzone) return 0.0f;
    
    float sign = (value > 0) ? 1.0f : -1.0f;
    return sign * (fabs(value) - deadzone) / (1.0f - deadzone);
}