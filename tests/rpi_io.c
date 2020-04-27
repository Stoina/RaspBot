#include "rpi_io.h"

struct bcm2835_peripheral gpio = {GPIO_BASE};

//Zugriff auf physikalischen Speicher über Datei dev/mem von Linux-Kernel
int map_peripherals(struct bcm2835_peripheral *p)
{
    //öffnen /dev/mem 
    if((p->mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC)) < 0)
    {
        printf("Fehler beim Öffnen von /dev/mem. Überprüfe Berechtigungen.\n");
        return -1;
    }

    //mmap creates a new mapping in the virtual address space
    p->map = mmap(
        NULL,
        BLOCK_SIZE,
        PROT_READ|PROT_WRITE,
        MAP_SHARED,
        p->mem_fd, //file descriptor zu phys.Speicher des virtual file '/dev/mem'
        p->addr_p   
    );

    if (p->map == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    p->addr = (volatile unsigned int *)p->map;
    return 0;   
}

void unmap_peripherals(struct bcm2835_peripheral *p)
{
    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}

void delay(ulong seconds, ulong nanoseconds)
{
    struct timespec req = {0};
    req.tv_sec = seconds;
    req.tv_nsec = nanoseconds; //;
    nanosleep(&req, NULL);  
}