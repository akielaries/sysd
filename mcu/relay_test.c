#include "stm.h"
#include <stdint.h>

// This is the symbol defined in the linker script.
extern uint32_t _end_of_ram;

// We'll implement main a bit further down but we need the symbol for
// the initial program counter / start address.
void main();

// This is our interrupt vector table that goes in the dedicated section.
// see linker_script.ld and 10.1.2 Interrupt and exception vectors
__attribute__((section(".interrupt_vectors"),
               used)) void (*const interrupt_vectors[])(void) = {
    // The first 32bit value is actually the initial stack pointer.
    // We have to cast it to a function pointer since the rest of the array
    // would all be function pointers to interrupt handlers.
    (void (*)(void))((uint32_t)&_end_of_ram),

    // The second 32bit value is the initial program counter aka. the
    // function you want to run first.
    main};

/**
 * @brief wait function
 *
 * @note right now targets the STM32F103x with ARM M3. High speed internal
 * oscillator (HSI) runs at 8MHz (default) where each loop iteration takes
 * about 1 second supposedly. TODO: verify this with some math... how can
 * clock speed be configured...
 * @note 7.2.2 HSI clock
 *
 * @param[in] time time in seconds to wait
 */
// void wait(float time) {
void wait() {
    // Do some NOPs for a while to pass some time.
    // for (uint32_t i = 0; i < (time * 2000000); ++i) __asm__ volatile ("nop");
    for (uint32_t i = 0; i < 2000000; ++i)
        __asm__ volatile("nop");
}

void clear_bits(volatile uint32_t *reg, uint32_t mask) {
    *reg &= ~mask;
}

void set_bits(volatile uint32_t *reg, uint32_t mask) {
    *reg |= mask;
}

void configure_gpio_pin(volatile uint32_t *reg,
                        uint32_t           pin_pos,
                        uint32_t           mode) {
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

    // So we want to set bit 4 of this register to 1 to enable port C but wheres
    // this register in memory? We're only being told that it is at address 0x18
    // within the memory for RCC configuration. So let's go back to section 3.3
    // to consult the memory map. There, "Reset and clock control RCC" is listed
    // at 0x4002 1000. Adding 0x18 we get 0x4002 1018.
    *((volatile uint32_t *)RCC_APB2ENR) |= (1 << 4); // 4th bit = IO Port C

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
    /*
        volatile uint32_t *gpio_crh = (volatile uint32_t *)GPIOC_CRH;

        // Start with the reset value
        *gpio_crh = GPIOC_CRH_RESET;

        // Configure pins 13, 14, and 15 as output, 50 MHz
        configure_gpio_pin(gpio_crh, GPIOC_P13, 0x3U);
        configure_gpio_pin(gpio_crh, GPIOC_P14, 0x3U);
        configure_gpio_pin(gpio_crh, GPIOC_P15, 0x3U);
    */
    // call via this method takes up 40-80 less bytes
    *((volatile uint32_t *)GPIOC_CRH) =
        ((GPIOC_CRH_RESET                  // reset val
                                           // push-pull output [CNF:00MODE:11]
          & ~(CRH_CLEAR_BITS << GPIOC_P13) // Clear out the bits for pin 13
          & ~(CRH_CLEAR_BITS << GPIOC_P14) & ~(CRH_CLEAR_BITS << GPIOC_P15))
         // set the bits (20-21) for P13 to 0011(0x3), leave other bits
         // unchanged set as output with 50MHz max. set pins with 0x03 = 0011;
         // 00=GPO push-pull,11=output @ max 50MHz
         |
         (CRH_MODE_OUT_50MHZ << GPIOC_P13) // Set both MODE+CNF bits for pin 13
         | (CRH_MODE_OUT_50MHZ << GPIOC_P14) |
         (CRH_MODE_OUT_50MHZ << GPIOC_P15));

    while (1) {
        // Set the output bit.
        // 9.2.5 Port bit set/reset register (GPIOx_BSRR) (x=A..G)
        // Address offset: 0x10
        // Reset value: 0x0000 0000
        // Finally, we need to toggle the pin between high and low to blink the
        // LED. We can use the trick described in section 9.1.2 to atomically
        // set/reset only the pin we're interested in. The BSRR register for
        // that purpose is at offset 0x10 (16) as described in section 9.2.5. We
        // need to set bit 13 to make PC13 high and bit 29 (pin + 0x10) to make
        // it low respectively.
        *((volatile uint32_t *)GPIOC_BSRR) = (1U << P13_HI);
        *((volatile uint32_t *)GPIOC_BSRR) = (1U << P14_HI);
        wait();
        *((volatile uint32_t *)GPIOC_BSRR) = (1U << P15_HI);
        wait();

        // Reset it again.
        *((volatile uint32_t *)GPIOC_BSRR) = (1U << P13_LO);
        *((volatile uint32_t *)GPIOC_BSRR) = (1U << P14_LO);
        *((volatile uint32_t *)GPIOC_BSRR) = (1U << P15_LO);
        wait();
    }
}

void main() {
    blink();
}
