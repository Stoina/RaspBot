// define gpio select functions in 3-Bit
#ifndef GPIO_SEL_FUNCTIONS
#define GPIO_SEL_FUNCTIONS

#define GPIO_FUNC_IN  0b000
#define GPIO_FUNC_OUT 0b001
#define GPIO_FUNC_ALTERNATE_0 0b100
#define GPIO_FUNC_ALTERNATE_1 0b101
#define GPIO_FUNC_ALTERNATE_2 0b110
#define GPIO_FUNC_ALTERNATE_3 0b111
#define GPIO_FUNC_ALTERNATE_4 0b011
#define GPIO_FUNC_ALTERNATE_5 0b010

#endif

#ifndef GPIO_STATES
#define GPIO_STATES

#define GPIO_STATE_LOW  0
#define GPIO_STATE_HIGH 1

#endif

class GPIOBoard {
    private:
        int mem_base;
        int mem_fd;
        int gpio_base;
        unsigned int* gpio_mmap;

        int get_GPSEL_register(int gpio_pin);
        int get_GPSET_register(int gpio_pin);
        int get_GPCLR_register(int gpio_pin);
        int get_GPLEV_register(int gpio_pin);
        
    public:
        GPIOBoard();

        void greeting();

        int setup_board();

        int open_mem_file();
        int open_gpiomem_file();
        int open_mem_file(const char* mem_file_name);
        int map_memory();
        
        int setup_pin(int gpio_pin, int gpio_func);
        
        int set_pin_low(int gpio_pin);
        int set_pin_high(int gpio_pin);
        int set_pin_state(int gpio_pin, int gpio_state);
        int read_pin_state(int gpio_pin);
};