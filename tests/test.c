#include "rpi_io.h"

#define PIN     (26)

int main(){

    if(map_peripherals(&gpio) == -1)
    {
        printf("Fehler beim Mapping des physikalischen GPIO-Registers in den virtuellen Speicherbereich.\n");
        return -1;
    }

    //Pin 17 als Output-Pin definieren
    INP_GPIO(PIN);
    OUT_GPIO(PIN);

    while(1)
    {
        GPIO_SET = 1 << PIN;
        sleep(1);

        GPIO_CLR = 1 << PIN;
        sleep(1);
    }

    return 0;
}