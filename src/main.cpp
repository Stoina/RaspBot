#include <iostream>

#include "gpio/gpioboard.h"

#include "controlsystem/engine.h"
#include "controlsystem/ultrasonic_sensor.h"

int main() 
{
    std::cout << "Starting raspbot-control-system..." << std::endl;

    GPIOBoard board;
    
    if (board.setup_board() >= 0)
    {
        board.setup_pin(GPIO_FUNC_OUT, 2);
        board.set_pin_state(2, GPIO_STATE_HIGH);
    }

    return 0;
}