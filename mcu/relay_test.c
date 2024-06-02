#include <stdint.h>
#include "stm.h"

// This is the symbol defined in the linker script.
extern unsigned long _end_of_ram;

// We'll implement main a bit further down but we need the symbol for
// the initial program counter / start address.
void main();

// This is our interrupt vector table that goes in the dedicated section.
__attribute__ ((section(".interrupt_vectors"), used))
void (* const interrupt_vectors[])(void) = {
    // The first 32bit value is actually the initial stack pointer.
    // We have to cast it to a function pointer since the rest of the array
    // would all be function pointers to interrupt handlers.
    (void (*)(void))((unsigned long)&_end_of_ram),

    // The second 32bit value is the initial program counter aka. the
    // function you want to run first.
    main
};

void wait() {
    // Do some NOPs for a while to pass some time.
    for (unsigned int i = 0; i < 2000000; ++i) __asm__ volatile ("nop");
}

void clear_bits(volatile unsigned int *reg, unsigned int mask) {
    *reg &= ~mask;
}

void set_bits(volatile unsigned int *reg, unsigned int mask) {
    *reg |= mask;
}

void configure_gpio_pin(volatile unsigned int *reg, unsigned int pin_pos, unsigned int mode) {
    // Clear the bits for the pin
    clear_bits(reg, 0xfU << pin_pos);

    // Set the bits for the pin
    set_bits(reg, mode << pin_pos);
}


void blink() {
    // Enable port C clock gate.
    // 7.3.7 APB2 peripheral clock enable register (RCC_APB2ENR)
    // Address: 0x18
    // Reset value: 0x0000 0000

    //So we want to set bit 4 of this register to 1 to enable port C but wheres 
    //this register in memory? We're only being told that it is at address 0x18 
    //within the memory for RCC configuration. So let's go back to section 3.3 to 
    //consult the memory map. There, "Reset and clock control RCC" is listed 
    //at 0x4002 1000. Adding 0x18 we get 0x4002 1018.
    *((volatile unsigned int *) RCC_APB2ENR) |= (1 << 4);

    // Configure GPIO C pins 13,14,15 as outputs.
    // 9.2.2 Port configuration register high (GPIOx_CRH) (x=A..G)
    // Address offset: 0x04
    // Reset value: 0x4444 4444
    // The bits for pin 13 are 20-23. Note the reset value 0x4444 4444: We'll 
    // have to leave the other bits with those values to avoid changing the 
    // config of any other pins. The section specifies an address offset of 0x04
    // but what is the base for this? Our trusty memory map in section 3.3 
    // points us at 0x4001 1000 for GPIO Port C, so we get 0x4001 1004.
    // The bits for pin 14 are 24-27

    volatile unsigned int *gpio_crh = (volatile unsigned int *)GPIOC_CRH;

    // Start with the reset value
    *gpio_crh = GPIOC_CRH_RESET;

    // Configure pins 13, 14, and 15 as output, 50 MHz
    configure_gpio_pin(gpio_crh, GPIOC_P13, 0x3U);
    configure_gpio_pin(gpio_crh, GPIOC_P14, 0x3U);
    configure_gpio_pin(gpio_crh, GPIOC_P15, 0x3U);

/*
    *((volatile unsigned int *) GPIOC_CRH) = ((GPIOC_CRH_RESET // The reset val
        // push-pull output [CNF:00MODE:11]
        // shift 4bit 1111 val to start register of P13
        // clear 4 bits (20-23) for P13, leave other bits unchanged
        & ~(0xfU << GPIOC_P13)  // Clear out the bits for pin 13
        & ~(0xfU << GPIOC_P14)  // Clear out the bits for pin 14
        & ~(0xfU << GPIOC_P15)) // Clear out the bits for pin 15
        // set the bits (20-21) for P13 to 11, leave other bits unchanged
        |  (0x3U << GPIOC_P13)  // Set both MODE bits for pin 13, leave CNF @ 0
        |  (0x3U << GPIOC_P14)  // Set both MODE bits for pin 14
        |  (0x3U << GPIOC_P15)); // Set both MODE bits for pin 15
    // Step 1: Define the original register value with reset
    volatile unsigned int reg_value = GPIOC_CRH_RESET;

    // Step 2: Create masks to clear bits for pin 13, pin 14, and pin 15
    unsigned int mask_p13 = 0xfU << GPIOC_P13; // 0xf (b1111) shifted to bits 20-23
    unsigned int mask_p14 = 0xfU << GPIOC_P14; // 0xf shifted to bits 24-27
    unsigned int mask_p15 = 0xfU << GPIOC_P15; // 0xf shifted to bits 28-31

    // Step 3: Clear the bits for pin 13, pin 14, and pin 15 in the register value
    reg_value &= ~mask_p13; // Clear bits 20-23
    reg_value &= ~mask_p14; // Clear bits 24-27
    reg_value &= ~mask_p15; // Clear bits 28-31

    // Step 4: Create values to set the mode bits for pin 13, pin 14, and pin 15
    unsigned int value_p13 = 0x3U << GPIOC_P13; // 0x3 (b11) shifted to bits 20-21
    unsigned int value_p14 = 0x3U << GPIOC_P14; // 0x3 shifted to bits 24-25
    unsigned int value_p15 = 0x3U << GPIOC_P15; // 0x3 shifted to bits 28-29

    // Step 5: Set the mode bits for pin 13, pin 14, and pin 15 in the register value
    reg_value |= value_p13; // Set bits 20-21 to 11
    reg_value |= value_p14; // Set bits 24-25 to 11
    reg_value |= value_p15; // Set bits 28-29 to 11

    // Step 6: Write the final value back to the register
    *((volatile unsigned int *) GPIOC_CRH) = reg_value;
*/
    //while (1) {
        // Set the output bit.
        // 9.2.5 Port bit set/reset register (GPIOx_BSRR) (x=A..G)
        // Address offset: 0x10
        // Reset value: 0x0000 0000
        //Finally, we need to toggle the pin between high and low to blink the LED. 
        //We can use the trick described in section 9.1.2 to atomically set/reset 
        //only the pin we're interested in. The BSRR register for that purpose is 
        //at offset 0x10 (16) as described in section 9.2.5. We need to set bit 13 
        //to make PC13 high and bit 29 (pin + 0x10) to make it low respectively.
        *((volatile unsigned int *)0x40011010) = (1U << 13);
        *((volatile unsigned int *)0x40011010) = (1U << 14);
        wait();
        *((volatile unsigned int *)0x40011010) = (1U << 15);
        wait();

        // Reset it again.
        *((volatile unsigned int *)0x40011010) = (1U << 29);
        *((volatile unsigned int *)0x40011010) = (1U << 30);
        *((volatile unsigned int *)0x40011010) = (1U << 31);
        wait();
    //}
}

void main() {
    blink();

    

}
