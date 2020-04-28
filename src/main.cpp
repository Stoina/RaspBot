#include "main.h"

int main() 
{
    std::cout << "Starting raspbot-control-system..." << std::endl;

    try 
    {
        GPIOBoard board;
        if (board.setup_board() >= 0)
        {
            UltrasonicSensor ultrasonicSensor;
            ultrasonicSensor.initialize_board(&board, 21, 20);

            while(true)
            {
                std::cout << ultrasonicSensor.read_distance() << std::endl;
                sleep(3);
            }
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