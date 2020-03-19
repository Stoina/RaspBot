#include <iostream>
#include <fcntl.h>

#include "rpigpio.h"

#ifdef __unix__

#define IsUnixOs

#include <sys/types.h>
#include <sys/mman.h>

#endif

struct GPIOBoard 
{
    int addr_base;
    int* mem_fd;
    void* memory_map;
}

void* initialize_bcm_board(int* mem_fd)
{
    int fd = -1;

    if ((fd = open_dev_mem_file(mem_fd)) >= 0)
    {
        return map_mem(fd, mem_fd);
    }
    else if ((fd = open_dev_gpiomem_file(mem_fd)) >= 0)
    {
        return map_mem(fd, mem_fd);
    }
    else 
    {
        std::cout << "Error occurred while opening file..." << std::endl;
    }
}

int open_dev_mem_file(int* ptr_mem_fd)
{
    return open_mem_file("/dev/mem", ptr_mem_fd);
}

int open_dev_gpiomem_file(int* ptr_mem_fd)
{
    return open_mem_file("/dev/gpiomem", ptr_mem_fd);
}

int open_mem_file(const char* dev_mem_file_name, int* ptr_mem_fd)
{
    int fd = open_mem_file(dev_mem_file_name);
    ptr_mem_fd = &fd;
    return fd;
}

int open_mem_file(const char* dev_mem_file_name)
{
    std::cout << std::endl;
    std::cout << "Try to open file: " << dev_mem_file_name << "..." << std::endl;
    
    int fd = open(dev_mem_file_name, O_RDWR|O_SYNC);
    
    if (fd < 0)
    {
        std::cout << "Error while mapping physical gpio-register in virtual memory. File descriptor: " << fd << std::endl;
    }
    else
    {
        std::cout << "File opened successfully: " << dev_mem_file_name << " Descriptor: " << fd << std::endl;
    }
    
    return fd;
}

void* map_mem(int mem_fd, int* ptr_mem_fd)
{
    std::cout << std::endl;
    std::cout << "Map memory.." << std::endl;

    void* map = mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, GPIO_BASE);
    
    if (map == (__caddr_t)-1)
    {
        std::cout << "Error mapping memory" << std::endl;
    }
    else
    {
        std::cout << "Successfully mapped memory" << std::endl;
    }

    return map;
}

void set_test_pin_on(void* map)
{
    int pin = 2;
    int pin_register = map + (2 / 10);
    int bit_position =  2* 3;

    std::cout << "Pin register: " << pin_register;
    std::cout << "Bit position: " << bit_position;
    std::cout << "Bit mask: " << (u_int32_t)(GPIO_OUT << bit_position);;
    
    *(pin_register) = (u_int32_t)(GPIO_OUT << bit_position);
}

void set_test_pin_output_high()
{

}

void set_test_pin_output_low()
{
    
}