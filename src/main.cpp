#include <iostream>

#include "gpio/gpioboard.h"

#include "controlsystem/engine.h"
#include "controlsystem/ultrasonic_sensor.h"

int main() 
{
    std::cout << "Starting raspbot-control-system..." << std::endl;

    try { 
        GPIOBoard board;
        if (board.setup_board() >= 0)
        {
            board.setup_pin(9, GPIO_FUNC_OUT);
            board.set_pin_low(9);
        }
        else
        {
            std::cout << "Starting raspbot-control system failed..." << std::endl;
        }
    } 
    catch (const std::exception& e)
    {
        std::cout << "Starting raspbot-control system failed..." << std::endl;
    }
    
    return 0;
}