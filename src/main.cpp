#include <iostream>
#include "gpio/rpigpio.h"
#include "controlsystem/engine.h"
#include "controlsystem/ultrasonic_sensor.h"

int main() 
{
    std::cout << "Starting raspbot-control-system" << std::endl;
    
    int* mem_fd;
    void* map = initialize_bcm_board(mem_fd);
    set_test_pin_on(map);
    
    return 0;
}