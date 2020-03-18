#include <iostream>
#include "gpio/rpigpio.h"
#include "controlsystem/engine.h"
#include "controlsystem/ultrasonic_sensor.h"

int main() 
{
    std::cout << "Starting raspbot-control-system";

    int* mem_fd;
    
    initialize_bcm_board(mem_fd);
    unmap_memory(mem_fd);

    return 0;
}