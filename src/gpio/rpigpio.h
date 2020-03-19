#ifndef BLOCK_SIZE
#define BLOCK_SIZE (4*1024)
#endif

#ifndef BCM2708_GPIO
#define BCM2708_GPIO 0x20000000
#endif

// virtual address from BCM2835 CPU
// in the documentation the address is 0x7E.... but because the MMU we must change to 0x20....
#ifndef BCM2835_GPIO
#define BCM2835_GPIO 0x200000
#endif

#define GPIO_BASE (BCM2708_GPIO + BCM2835_GPIO)

// define gpio select functions in 3-Bit
#ifndef GPIO_SEL_FUNCTIONS
#define GPIO_SEL_FUNCTIONS

#define GPIO_OUT 0b000
#define GPIO_IN  0b001
#define GPIO_ALTERNATE_0 0b100
#define GPIO_ALTERNATE_1 0b101
#define GPIO_ALTERNATE_2 0b110
#define GPIO_ALTERNATE_3 0b111
#define GPIO_ALTERNATE_4 0b011
#define GPIO_ALTERNATE_5 0b010

#endif

void* initialize_bcm_board(int* mem_fd);
int open_dev_mem_file(int* ptr_mem_fd);
int open_dev_gpiomem_file(int* ptr_mem_fd);
int open_mem_file(const char* dev_mem_file_name, int* ptr_mem_fd);
int open_mem_file(const char* dev_mem_file_name);
void* map_mem(int mem_fd, int* ptr_mem_fd);
void set_test_pin_on(void* map);
void set_test_pin_output_high();
void set_test_pin_output_low();