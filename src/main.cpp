#include <iostream>
#include "gpio/rpigpio.h"
#include "controlsystem/engine.h"
#include "controlsystem/ultrasonic_sensor.h"

int main() 
{
    initialize_bcm_board();

    return 0;
}