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

    gpio_mmap = (uint32_t *)mmap(0, (4*1024), PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, (gpio_base + 0x00200000));
    
    if (gpio_mmap == MAP_FAILED)
    {
        std::cout << "Error mapping memory..." << std::endl;
    }
    else
    {
        std::cout << "Successfully mapped memory..." << std::endl;
        res = 0;
    }

    std::cout << std::endl;

    return res;
}

int GPIOBoard::setup_pin(int gpio_pin, int gpio_func)
{
    std::cout << "Set pin " << gpio_pin << " to function "  << gpio_func << "..." << std::endl;
    (gpio_func == GPIO_FUNC_IN) ? *(gpio_mmap + get_GPSEL_register(gpio_pin)) &= ~(gpio_func << (((gpio_pin) % 10) * 3)) : *(gpio_mmap + get_GPSEL_register(gpio_pin)) |= (gpio_func << (((gpio_pin) % 10) * 3));
    return 0;
}

int GPIOBoard::set_pin_low(int gpio_pin)
{
    std::cout << "Set pin " << gpio_pin << " low..." << std::endl;
    return set_pin_state(gpio_pin, GPIO_STATE_LOW);
}

int GPIOBoard::set_pin_high(int gpio_pin)
{
    std::cout << "Set pin " << gpio_pin << " high..." << std::endl;
    return set_pin_state(gpio_pin, GPIO_STATE_HIGH);
}

int GPIOBoard::set_pin_state(int gpio_pin, int gpio_state)
{
    *(gpio_mmap + ((GPIO_STATE_LOW == gpio_state) ? get_GPCLR_register(gpio_pin) : get_GPSET_register(gpio_pin))) = 1 << gpio_pin;
    return 0;
}

int GPIOBoard::read_pin_state(int gpio_pin)
{
    return (*(gpio_mmap + get_GPLEV_register(gpio_pin)) & 1 << gpio_pin) > 0 ? 1 : 0;
}

int GPIOBoard::get_GPSEL_register(int gpio_pin)
{
    return ((gpio_pin) / 10);
}

int GPIOBoard::get_GPSET_register(int gpio_pin)
{
    return (gpio_pin <= 32) ? 7 : 8;
}

int GPIOBoard::get_GPCLR_register(int gpio_pin)
{
    return (gpio_pin <= 32) ? 10 : 11;
}

int GPIOBoard::get_GPLEV_register(int gpio_pin)
{
    return (gpio_pin <= 32) ? 13 : 14;
}