#include "ultrasonic_sensor.h"

UltrasonicSensor::UltrasonicSensor()
{

}

int UltrasonicSensor::initialize_board(GPIOBoard* i_gpio_board, int trigger_pin, int echo_pin)
{
    gpio_board = i_gpio_board;
    initialize_trigger_pin(trigger_pin);
    initialize_echo_pin(echo_pin);

    return 0;
}

int UltrasonicSensor::initialize_trigger_pin(int trigger_pin)
{
    this->trigger_pin = trigger_pin;
    gpio_board->setup_pin(trigger_pin, GPIO_FUNC_OUT);
    return 0;
}

int UltrasonicSensor::initialize_echo_pin(int echo_pin)
{
    this->echo_pin = echo_pin;
    gpio_board->setup_pin(echo_pin, GPIO_FUNC_IN);
    return 0;
}

double UltrasonicSensor::read_distance()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;

    gpio_board->set_pin_high(trigger_pin);
    sleep(0.00001);
    gpio_board->set_pin_low(trigger_pin);

    while(gpio_board->read_pin_state(echo_pin) == 0)
    {
        start = std::chrono::high_resolution_clock::now();
    }

    while(gpio_board->read_pin_state(echo_pin) == 1)
    {
        end = std::chrono::high_resolution_clock::now();
    }

    return (std::chrono::duration_cast<std::chrono::duration<double>>(end-start).count() * 34300 / 2);
}