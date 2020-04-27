#include <stdio.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>
#include <time.h>

//phys. Adressraum/Startadresse für Periperiekomponenten, wie USB, GPIO
#define BCM2708_PERI_BASE   0x20000000 
//phys. Adressraum/Startadresse für GPIO
#define GPIO_BASE           (BCM2708_PERI_BASE + 0x200000)

#define BLOCK_SIZE          (4*1024)

//IO Zugriff Struct
struct bcm2835_peripheral 
{
    unsigned long addr_p;
    int mem_fd; //file descriptor to referer memory file /dev/mem
    void *map;
    volatile unsigned int *addr;
};
//volatile sorgt dafür, dass Wert im aus Hauptspeicher geholt wird und
//nicht aus irgend ein Register in CPU; 

extern struct bcm2835_peripheral gpio;

// Makros für GPIO-Zugriff; immer INP_GPIO(x) vor OUT_GPIO(x) benutzen
#define INP_GPIO(g)   *(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)   *(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET  *(gpio.addr + 7)  // setzt Bits die 1 sind und ignoriert Bits die 0 sind
#define GPIO_CLR  *(gpio.addr + 10) // löscht Bits die 1 sind und ignoriert Bits die 0 sind

#define GPIO_READ(g)  *(gpio.addr + 13) &= (1<<(g))
#define GPIO_PULL  *(gpio.addr + 37)  //Pull up oder Pull Down Aktivierungen
#define GPIO_PULLCLK(g) *(gpio.addr + 38) &= (1<<(g)) //Clock pull up oder pull down

//Zugriff auf physikalischen Speicher über Datei dev/mem von Linux-Kernel
extern int map_peripherals(struct bcm2835_peripheral *p);
extern void unmap_peripherals(struct bcm2835_peripheral *p);

extern void delay(ulong seconds, ulong nanoseconds);