#ifndef __STM_H__
#define __STM_H__

// size of each register segment in the register map
#define REG_SIZE 0x3FF

/** Enumeration for RCC (Reset & Clock Control) registers */
typedef enum {
    // start address of RCC registers
    RCC_START   = 0x40021000,
    // APB2 (advance perhipheral bus) clock enable register
    RCC_APB2ENR = RCC_START + 0x18,
    // end address of RCC registers
    RCC_END     = 0x400213FF,
    // RCC reset value
    RCC_RESET   = 0x00000000,

} stm32f103x_rcc_reg;

/** Enumeration for GPIO Port A registers */
typedef enum {
    // start address for GPIO port A
    GPIOA_START = 0x40010800,
    // end address for GPIO port A
    GPIOA_END   = 0x40010BFF,

} stm32f103x_gpioa_reg;

/** Enumeration for GPIO Port B registers */
typedef enum {
    // start address for GPIO port B
    GPIOB_START = 0x40010C00,
    // end address for GPIO port B
    GPIOB_END   = 0x40010FFF,

} stm32f103x_gpiob_reg;

/** Define for Config Register High configuration bits */
// CNFy[1:0] BITS
// input mode (MODE[1:0]=00)
#define CRH_CNF_IN_ANLG 0x03    // (0011) analog mode
#define CRH_CNF_IN_FP   0x07    // (0111) floating input (reset state)
#define CRH_CNF_IN_PUPD 0x0B    // (1011) input pull up/down
#define CRH_CNF_IN_RSRV 0x0F    // ([1111]) reserved 

// output mode (MODE[1:0]>00)
#define CRH_CNF_OUT_GPPP 0x03   // (0011) General purpose output push-pull
#define CRH_CNF_OUT_GPOD 0x07   // (0111) General purpose output Open-drain
#define CRH_CNF_OUT_AFPP 0x0B   // (1011) Alternate function output Push-pull
#define CRH_CNF_OUT_AFOD 0x0F   // ([1111]) Alternate function output Open-drain

// MODEy[1:0] BITS
#define CRH_MODE_OUT_10mhz  0x03    // ([00]11) input mode (reset state)
#define CRH_MODE_OUT_10MHz  0x07    // ([01]11) Output mode, max speed 10 MHz
#define CRH_MODE_OUT_2MHz   0x0B    // ([10]11) Output mode, max speed 2 MHz
#define CRH_MODE_OUT_50MHz  0x0F    // ([1111]) Output mode, max speed 50 MHz

/** Enumeration for GPIO Port C registers */
typedef enum {
    // start address for GPIO port C
    GPIOC_START     = 0x40011000,
    // Configure Register High
    GPIOC_CRH       = GPIOC_START + 0x04,

    // GPIO C Pin 13-15
    GPIOC_P13       = 20,   // 20-23
    GPIOC_P14       = 24,   // 24-27
    GPIOC_P15       = 28,   // 28-31
    
    // CRH Reset
    GPIOC_CRH_RESET = 0x44444444,

    // end address for GPIO port C
    GPIOC_END       = 0x400113FF,

} stm32f103x_gpioc_reg;

/** Enumeration for GPIO Port D registers */
typedef enum {
    // start address for GPIO port D
    GPIOD_START = 0x40011400,
    // end address for GPIO port D
    GPIOD_END   = 0x400117FF,

} stm32f103x_gpiod_reg;

/** Enumeration for GPIO Port E registers */
typedef enum {
    // start address for GPIO port E
    GPIOE_START = 0x40011800,
    // end address for GPIO port E
    GPIOE_END   = 0x40011BFF,

} stm32f103x_gpioe_reg;

/** Enumeration for GPIO Port F registers */
typedef enum {
    // start address for GPIO port F
    GPIOF_START = 0x40011C00,
    // end address for GPIO port F
    GPIOF_END   = 0x40011FFF,

} stm32f103x_gpiof_reg;

/** Enumeration for GPIO Port G registers */
typedef enum {
    // start address for GPIO port G
    GPIOG_START = 0x40012000,
    // end address for GPIO port G
    GPIOG_END   = 0x400123FF,

} stm32f103x_gpiog_reg;

#endif
