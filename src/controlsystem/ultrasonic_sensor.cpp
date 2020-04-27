#pragma once

#include "ultrasonic_sensor.h"

UltrasonicSensor::UltrasonicSensor()
{

}

int UltrasonicSensor::initialize_board(GPIOBoard* i_gpio_board)
{
    gpio_board = i_gpio_board;
    return gpio_board->setup_board();
}

int UltrasonicSensor::initialize_trigger_pin(int trigger_pin)
{
    return 0;
}

int UltrasonicSensor::initialize_echo_pin(int echo_pin)
{
    return 0;
}