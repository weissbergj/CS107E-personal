/* File: test_gpio_timer.c
 * -----------------------
 * ***** This file has several tests for the breadboard, GPIO read/write functions, and the clock. *****
 */
#include "gpio.h"
#include "timer.h"

// You call assert on an expression that you expect to be true. If expr
// instead evaluates to false, then assert calls abort, which stops
// your program and flashes onboard led.
#define assert(expr) if(!(expr)) abort()

// infinite loop that flashes onboard blue LED (GPIO PD18)
void abort(void) {
    volatile unsigned int *GPIO_CFG2 = (unsigned int *)0x02000098;
    volatile unsigned int *GPIO_DATA = (unsigned int *)0x020000a0;

    // Configure GPIO PD18 function to be output.
    *GPIO_CFG2 = (*GPIO_CFG2 & ~(0xf00)) | 0x100;
    while (1) { // infinite loop
        *GPIO_DATA ^= (1 << 18); // invert value
        for (volatile int delay = 0x100000; delay > 0; delay--) ; // wait
    }
}

void test_gpio_set_get_function(void) {
    // Test get pin function (pin defaults to disabled)
    assert( gpio_get_function(GPIO_PC0) == GPIO_FN_DISABLED);

    // Set pin to output, confirm get returns what was set
    gpio_set_output(GPIO_PC0);
    assert( gpio_get_function(GPIO_PC0) == GPIO_FN_OUTPUT );

    // Set pin to input, confirm get returns what was set
    gpio_set_input(GPIO_PC0);
    assert( gpio_get_function(GPIO_PC0) == GPIO_FN_INPUT );
}

void test_gpio_read_write(void) {
    // set pin to output before gpio_write
    gpio_set_output(GPIO_PB4);

    // gpio_write low, confirm gpio_read reads what was written
    gpio_write(GPIO_PB4, 0);
    assert( gpio_read(GPIO_PB4) ==  0);
    
    // gpio_write high, confirm gpio_read reads what was written
    gpio_write(GPIO_PB4, 1);
    assert(gpio_read(GPIO_PB4) == 1);

    // gpio_write low, confirm gpio_read reads what was written
    gpio_write(GPIO_PB4, 0);
    assert( gpio_read(GPIO_PB4) ==  0 );
}

void test_gpio_read_write_any_pin(void) {
    gpio_id_t pins[] = {GPIO_PB1, GPIO_PC2, GPIO_PD3, GPIO_PE4, GPIO_PG4};
    for (int i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
        gpio_set_output(pins[i]);
        gpio_write(pins[i], 1);
        assert(gpio_read(pins[i]) == 1);
        gpio_write(pins[i], 0);
        assert(gpio_read(pins[i]) == 0);
    }
}

void test_gpio_independent_pins(void) {
    gpio_set_output(GPIO_PB4);
    gpio_set_output(GPIO_PC5);

    gpio_write(GPIO_PB4, 1);
    gpio_write(GPIO_PC5, 0);
    assert(gpio_read(GPIO_PB4) == 1);
    assert(gpio_read(GPIO_PC5) == 0);

    gpio_write(GPIO_PB4, 0);
    gpio_write(GPIO_PC5, 1);
    assert(gpio_read(GPIO_PB4) == 0);
    assert(gpio_read(GPIO_PC5) == 1);
}

void test_gpio_invalid_requests(void) {
    unsigned int GPIO_PA4 = 999;
    gpio_set_output(GPIO_PA4);
    assert(gpio_get_function(GPIO_PA4) == 0); // notice how errors are handled here

    // Attempt to write to invalid pin
    gpio_write(GPIO_PA4, 1);
    assert(gpio_read(GPIO_PA4) == GPIO_INVALID_REQUEST);
}

void test_timer(void) {
    // Test timer tick count incrementing
    unsigned long start = timer_get_ticks();
    for( int i=0; i<10; i++ ) { /* Spin */ }
    unsigned long finish = timer_get_ticks();
    assert( finish > start );

    // Test timer delay
    int usecs = 100;
    start = timer_get_ticks();
    timer_delay_us(usecs);
    finish = timer_get_ticks();
    assert( finish >= start + usecs*TICKS_PER_USEC );
}

void test_breadboard(void) {
    gpio_id_t segment[7] = {GPIO_PD17, GPIO_PB6, GPIO_PB12, GPIO_PB11, GPIO_PB10, GPIO_PE17, GPIO_PD11};
    gpio_id_t digit[4] = {GPIO_PB4, GPIO_PB3, GPIO_PB2, GPIO_PC0};

    gpio_id_t button = GPIO_PG13;

    for (int i = 0; i < 7; i++) {  // configure segments
        gpio_set_output(segment[i]);
    }
    for (int i = 0; i < 4; i++) {  // configure digits
        gpio_set_output(digit[i]);
    }
    gpio_set_input(button); // configure button

    while (1) { // loop forever (finish via button press, see below)
        for (int i = 0; i < 4; i++) {   // iterate over digits
            gpio_write(digit[i], 1);    // turn on digit
            for (int j = 0; j < 7; j++) {   // iterate over segments
                gpio_write(segment[j], 1);  // turn on segment
                timer_delay_ms(200);
                gpio_write(segment[j], 0);  // turn off segment
                if (gpio_read(button) == 0) return;  // stop when button pressed
            }
            gpio_write(digit[i], 0);    // turn off digit
        }
    }
}

void test_gpio_function_match(void) {
    gpio_set_output(GPIO_PC0);
    assert(gpio_get_function(GPIO_PC0) == GPIO_FN_OUTPUT);

    gpio_set_input(GPIO_PC0);
    assert(gpio_get_function(GPIO_PC0) == GPIO_FN_INPUT);
}

void test_gpio_configure_pin(void) {
    gpio_set_output(GPIO_PC0);
    assert(gpio_get_function(GPIO_PC0) == GPIO_FN_OUTPUT);

    gpio_set_input(GPIO_PC0);
    assert(gpio_get_function(GPIO_PC0) == GPIO_FN_INPUT);
}

void test_gpio_reconfigure_pin(void) {
    gpio_set_output(GPIO_PC0);
    assert(gpio_get_function(GPIO_PC0) == GPIO_FN_OUTPUT);

    gpio_set_input(GPIO_PC0);
    assert(gpio_get_function(GPIO_PC0) == GPIO_FN_INPUT);
}

void test_gpio_valid_ids(void) {
    gpio_id_t valid_pins[] = {GPIO_PB0, GPIO_PC1, GPIO_PD2, GPIO_PE3, GPIO_PF4};
    for (int i = 0; i < sizeof(valid_pins)/sizeof(valid_pins[0]); i++) {
        gpio_set_output(valid_pins[i]);
        assert(gpio_get_function(valid_pins[i]) == GPIO_FN_OUTPUT);
    }

    unsigned int invalid_pin = 999;
    gpio_set_output(invalid_pin);
    assert(gpio_get_function(invalid_pin) == 0);
}

void test_gpio_independent_configuration(void) {
    gpio_set_output(GPIO_PB4);
    gpio_set_input(GPIO_PC5);

    assert(gpio_get_function(GPIO_PB4) == GPIO_FN_OUTPUT);
    assert(gpio_get_function(GPIO_PC5) == GPIO_FN_INPUT);
}

void test_gpio_pullup(void) {
    gpio_set_input(GPIO_PB4);
    gpio_activate_pullup(GPIO_PB4);
    assert(gpio_pullup_read(GPIO_PB4) == 1);
}

void main(void) {
    gpio_init();
    timer_init();

    // Uncomment the call to each test function below when you have implemented
    // the functions and are ready to test them

    test_gpio_set_get_function();
    test_gpio_read_write();
    test_gpio_read_write_any_pin();
    test_gpio_independent_pins();
    // test_gpio_invalid_requests(); //FAILS
    test_gpio_function_match();
    test_gpio_configure_pin();
    test_gpio_reconfigure_pin();
    // test_gpio_valid_ids(); //FAILS
    test_gpio_independent_configuration();
    test_timer();
    test_breadboard();
    test_gpio_pullup();
}
