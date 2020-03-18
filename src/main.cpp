#include <iostream>
#include "gpio/rpigpio"

int main() 
{
    std::cout << "Hello World - RaspBot" << std::endl;
    
    std::cout << "Initialize Gpios..." << std::endl;
    initialize_bcm_board();
}