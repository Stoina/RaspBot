#include "gpioboard.h"

#include <iostream>
#include <bitset>
#include <fcntl.h>

#ifdef __unix__

#define IsUnixOs

#include <sys/types.h>
#include <sys/mman.h>

#endif

GPIOBoard::GPIOBoard() 
{

}

int GPIOBoard::setup_board() 
{
    int res = -1;

    if (open_mem_file() >= 0)
    {
        res = map_memory();
    }
    else if (open_gpiomem_file() >= 0)
    {
        res = map_memory();
    }
    else 
    {
        std::cout << "Error occurred while opening file..." << std::endl;
    }

    return res;
}

int GPIOBoard::open_mem_file()
{
    return open_mem_file("/dev/mem");
}

int GPIOBoard::open_gpiomem_file()
{
    return open_mem_file("/dev/gpiomem");
}

int GPIOBoard::open_mem_file(const char* mem_file_name)
{
    int res = -1;

    std::cout << std::endl;
    std::cout << "Try to open file: " << mem_file_name << "..." << std::endl;
    
    mem_fd = open(mem_file_name, O_RDWR|O_SYNC);
    
    if (mem_fd < 0)
    {
        std::cout << "Error while mapping physical gpio-register in virtual memory. File descriptor: " << mem_fd << std::endl;
    }
    else
    {
        std::cout << "File opened successfully: " << mem_file_name << " Descriptor: " << mem_fd << std::endl;
        res = 1;
    }
    
    return res;
}

int GPIOBoard::map_memory()
{
    int res = -1;

    std::cout << std::endl;
    std::cout << "Map memory.." << std::endl;

    gpio_mmap = mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, GPIO_BASE);
    
    if (gpio_mmap == (__caddr_t)-1)
    {
        std::cout << "Error mapping memory..." << std::endl;
    }
    else
    {
        std::cout << "Successfully mapped memory..." << std::endl;
        res = 0;
    }

    return res;
}

int GPIOBoard::setup_pin(int gpio_func, int gpio_pin)
{
    *((uint32_t*)gpio_mmap + get_GPSEL_register(gpio_pin)) ^= (uint32_t)(gpio_func << (gpio_pin * 3));

    return 0;
}

int GPIOBoard::set_pin_low(int gpio_pin)
{
    std::cout << "Set pin " << gpio_pin << " low...";

    *((uint32_t*)gpio_mmap + get_GPCLR_register(gpio_pin)) ^= 1 << (gpio_pin & 31);

    return 0;
}

int GPIOBoard::set_pin_high(int gpio_pin)
{
    std::cout << "Set pin " << gpio_pin << " high...";

    *((uint32_t*)gpio_mmap + get_GPSET_register(gpio_pin)) ^= 1 << (gpio_pin & 31);
    
    return 0;
}

int GPIOBoard::set_pin_state(int gpio_pin, int gpio_state)
{
    if (GPIO_STATE_LOW == gpio_state)
        set_pin_low(gpio_pin);

    else if (GPIO_STATE_HIGH == gpio_state)
        set_pin_high(gpio_pin);

    return 0;
}

int GPIOBoard::get_GPSEL_register(int gpio_pin)
{
    return gpio_pin / 10;
}

int GPIOBoard::get_GPSET_register(int gpio_pin)
{
    if (gpio_pin <= 32)
        return 7;

    else
        return 8;
}

int GPIOBoard::get_GPCLR_register(int gpio_pin)
{
    if (gpio_pin <= 32)
        return 10;

    else
        return 11;
}