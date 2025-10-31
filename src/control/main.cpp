#include "include/RobotController.hpp"
#include <iostream>
#include <signal.h>

RobotController* global_controller = nullptr;

void signal_handler(int signum) {
    std::cout << "\n\nReceived signal " << signum << " (Ctrl+C)" << std::endl;
    std::cout << "Emergency stopping robot..." << std::endl;
    
    if (global_controller) {
        global_controller->emergency_stop();
    }
    
    std::cout << "Exiting safely." << std::endl;
    exit(signum);
}

int main(int argc, char* argv[]) {
    try {
        std::cout << "=== JetRacer Robot Control System ===" << std::endl;
        std::cout << "Initializing hardware..." << std::endl;
        
        RobotController controller;
        global_controller = &controller;
        
        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
        
        std::cout << "Hardware initialized successfully!" << std::endl;
        std::cout << "Starting control loop..." << std::endl;
        std::cout << "Press Ctrl+C to exit safely\n" << std::endl;
        
        controller.run();
        
    } catch (const std::exception& e) {
        std::cerr << "\n!!! ERROR !!!" << std::endl;
        std::cerr << "Exception: " << e.what() << std::endl;
        std::cerr << "\nPossible causes:" << std::endl;
        std::cerr << "  - I2C hardware not connected (PCA9685 at 0x40 or 0x60)" << std::endl;
        std::cerr << "  - PS3 controller not connected (/dev/input/js0)" << std::endl;
        std::cerr << "  - Insufficient permissions (try: sudo ./robot_control)" << std::endl;
        std::cerr << "\nTroubleshooting:" << std::endl;
        std::cerr << "  1. Check I2C devices: i2cdetect -y 1" << std::endl;
        std::cerr << "  2. Check joystick: ls /dev/input/js*" << std::endl;
        std::cerr << "  3. Check permissions: groups $USER" << std::endl;
        return 1;
    }
    
    return 0;
}