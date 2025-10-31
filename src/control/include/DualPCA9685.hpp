#ifndef DUALPCA9685_HPP
#define DUALPCA9685_HPP

#include <cstdint>

class DualPCA9685 {
private:
    int fd_motor;
    int fd_servo;
    
    void write_byte(int fd, uint8_t reg, uint8_t val);
    void set_pwm(int fd, uint8_t channel, uint16_t on, uint16_t off);
    void init_pca(int fd, uint8_t prescaler);
    void set_pwm_duty(uint8_t channel, float duty_fraction);
    uint16_t angle_to_pwm(float angle);

public:
    DualPCA9685();
    ~DualPCA9685();

    void set_motor(float throttle);
    void set_servo_angle(float angle);
    void stop_motors();
    void stop_all();
};

#endif