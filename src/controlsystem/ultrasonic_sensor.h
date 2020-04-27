#include "../gpio/gpioboard.h"

class UltrasonicSensor 
{
    private: 
        GPIOBoard* gpio_board;
        int trigger_pin;
        

    public:
        UltrasonicSensor();
        int initialize_board(GPIOBoard* i_gpio_board);
        int initialize_trigger_pin(int trigger_pin);
        int initialize_echo_pin(int echo_pin);
};