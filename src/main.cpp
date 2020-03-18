#include <iostream>
#include "gpio/rpigpio.h"
#include "controlsystem/engine.h"
#include "controlsystem/ultrasonic_sensor.h"

int main() 
{
    std::cout << "Hello World - RaspBot" << std::endl;
    std::cout << "Initialize GPIOs..." << std::endl;

    hello_gpio();
    hello_engine();
    hello_ultrasonic();

    return 0;
}