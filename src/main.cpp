#include <iostream>
#include "gpio/rpigpio.h"
#include "controlsystem/engine.h"
#include "controlsystem/ultrasonic_sensor.h"

int main() 
{
    std::cout << "Hello World - RaspBot" << std::endl;
    
    hello_gpio();
    hello_engine();
    hello_ultrasonic();

    std::cout << "Initialize GPIOs..." << std::endl;

    initialize_bcm_board();

    return 0;
}