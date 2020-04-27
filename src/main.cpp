#include "main.h"

int main() 
{
    std::cout << "Starting raspbot-control-system..." << std::endl;

    try 
    {
        GPIOBoard board;
        if (board.setup_board() >= 0)
        {
            board.setup_pin(21, GPIO_FUNC_OUT);
            board.setup_pin(20, GPIO_FUNC_IN);

            while (true)
            {
                std::chrono::time_point<std::chrono::system_clock> start, end;

                board.set_pin_high(21);
                sleep(0.00001);
                board.set_pin_low(21);

                while(board.read_pin_state(20) == 0)
                {
                    start = std::chrono::high_resolution_clock::now();
                }

                while(board.read_pin_state(20) == 1)
                {
                    end = std::chrono::high_resolution_clock::now();
                }

                std::cout << (std::chrono::duration_cast<std::chrono::duration<double>>(end-start).count() * 34300 / 2) << std::endl;
                sleep(3);
            }
            
            /*std::cout << endtime - starttime << std::endl;    
            std::cout << ((difftime(endtime, starttime) * 34200) / 2) << std::endl;*/
            
            /*while (1) 
            {
                double d = distance(&board);
                std::cout << d  << std::endl;

                sleep(1);
            }*/
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

double distance(GPIOBoard* gpio_board)
{
    gpio_board->set_pin_high(21);

    time_t starttime;
    time_t endtime;
    
    while(gpio_board->read_pin_state(20) == 0)
        time(&starttime);

    while(gpio_board->read_pin_state(20) == 0)
        time(&endtime);

    return difftime(endtime, starttime);
}