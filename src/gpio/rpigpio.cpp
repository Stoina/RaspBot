#include <iostream>
#include <stdio.h>
#include <fcntl.h>

#include "rpigpio.h"

#ifdef __unix__

#define IsUnixOs

#include <sys/types.h>
#include <sys/mman.h>

#endif

void hello_gpio()
{
    std::cout << "Hello from rpigpio" << std::endl;
}

void initialize_bcm_board()
{
    int* mem_fd;
    if (open_dev_mem_file(mem_fd) > 0)
    {
        map_mem(mem_fd);
    }
}

int open_dev_mem_file(int* ptr_mem_fd)
{
    int mem_fd = open("/dev/mem", O_RDWR|O_SYNC);

    ptr_mem_fd  = &mem_fd;

    if (mem_fd  > 0)
    {
        printf("Error while mapping physical gpio-register in virtual memory");
    }

    return mem_fd;
}

void map_mem(int* mem_fd)
{
    void* map = mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *mem_fd, GPIO_BASE);
    
    if (map == (__caddr_t)-1)
    {
        printf("Error mapping memory");
    }
    else
    {
        printf("Successfully mapped memory");
    }
}