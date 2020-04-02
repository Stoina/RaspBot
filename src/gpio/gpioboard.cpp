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
    
    if (*gpio_mmap == -1)
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
    
    uint32_t* address = gpio_mmap + get_GPSEL_register(gpio_pin);
    
    std::cout << "Address: " << address << std::endl;

    uint32_t current_bitmask = *(address);
    std::cout << "Current address bitmask: " << std::bitset<32>(current_bitmask) << std::endl;
    
    uint32_t new_bitmask = gpio_func << (gpio_pin / 10 + ((gpio_pin % 10) * 3));
    std::cout << "New bitmask: " << std::bitset<32>(new_bitmask) << std::endl;

    if (gpio_func != 0b000 && current_bitmask == (current_bitmask | new_bitmask))
    {
        std::cout << "Current pin is already in use!" << std::endl;
    }
    else
    {
        *(address) |= new_bitmask & 32;
        std::cout << "New register bitmask: " << std::bitset<32>(*(address)) << std::endl;
    }

    std::cout << std::endl;
    
    return 0;
}

int GPIOBoard::set_pin_low(int gpio_pin)
{
    std::cout << "Set pin " << gpio_pin << " low..." << std::endl;

    set_pin_state(gpio_pin, GPIO_STATE_LOW);

    return 0;
}

int GPIOBoard::set_pin_high(int gpio_pin)
{
    std::cout << "Set pin " << gpio_pin << " high..." << std::endl;
    
    set_pin_state(gpio_pin, GPIO_STATE_HIGH);
    
    return 0;
}

int GPIOBoard::set_pin_state(int gpio_pin, int gpio_state)
{
    uint32_t address_register = 0;

    if (GPIO_STATE_LOW == gpio_state)
        address_register = get_GPCLR_register(gpio_pin);

    else if (GPIO_STATE_HIGH == gpio_state)
        address_register = get_GPSET_register(gpio_pin);

    std::cout << "Address register: " << address_register << std::endl;

    uint32_t* address = gpio_mmap + address_register;
    
    std::cout << "Address: " << address << std::endl;
    std::cout << "Current address bitmask:\t" << std::bitset<32>(*(address)) << std::endl;

    uint32_t new_bitmask = 1 << ((gpio_pin - 1) & 31);
    std::cout << "New bitmask:\t \t \t" << std::bitset<32>(new_bitmask) << std::endl;
    
    if (*(address) != (*(address) | new_bitmask))
    {
        *(address) ^= new_bitmask;
    }

    std::cout << "New address bitmask:\t \t" << std::bitset<32>(*(address)) << std::endl;

    return 0;
}

int GPIOBoard::get_GPSEL_register(int gpio_pin)
{
    return (gpio_pin / 10);
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