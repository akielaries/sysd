#ifndef __STM_H__
#define __STM_H__

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL))) 

// size of each register segment in the register map
#define REG_SIZE 0x3FF

// UART
#define UART1 0x40013800
#define UART2 0x40004400
#define UART3 0x40004800

/** Define for Config Register High configuration bits on pins 8-15 */
// CNFy[1:0] BITS
// input mode (MODE[1:0]=00)
#define CRH_CNF_IN_ANLG 0x0     // (00) analog mode
#define CRH_CNF_IN_FP   0x1     // (01) floating input (reset state)
#define CRH_CNF_IN_PUPD 0x2     // (10) input pull up/down
#define CRH_CNF_IN_RSRV 0x3     // (11) reserved

// output mode (MODE[1:0]>00, if mode is set to output)
#define CRH_CNF_OUT_GPPP 0x0    // (00) General purpose output push-pull
#define CRH_CNF_OUT_GPOD 0x1    // (01) General purpose output Open-drain
#define CRH_CNF_OUT_AFPP 0x2    // (10) Alternate function output Push-pull
#define CRH_CNF_OUT_AFOD 0x3    // ([11]) Alternate function output Open-drain

// MODEy[1:0] BITS
#define CRH_MODE_OUT_10mhz  0x0     // (00) input mode (reset state)
#define CRH_MODE_OUT_10MHZ  0x1     // (01) Output mode, max speed 10 MHz
#define CRH_MODE_OUT_2MHZ   0x2     // (10) Output mode, max speed 2 MHz
#define CRH_MODE_OUT_50MHZ  0x3     // (11) Output mode, max speed 50 MHz

#define CRH_CLEAR_BITS      0xF
#define OFFSET_GPIO_CRH     0x04
#define OFFSET_GPIO_BSRR    0x18

/** Enumeration for RCC (Reset & Clock Control) registers */
// 7.4.19 APB2 peripheral clock enable register (RCC_APB2ENR)
typedef enum {
    // start address of RCC registers
    RCC_START   = 0x40021000,
    // APB2 (advance perhipheral bus) clock enable register
    RCC_APB2ENR = RCC_START + 0x60,
    
    // clock enable for GPIO
    RCC_AHB2ENR = RCC_START + 0x4C,

    // clock enable bit for GPIO port A
    RCC_GPIOA_EN = 0,
    // clock enable bit for GPIO port B
    RCC_GPIOB_EN = 1,

    // end address of RCC registers
    RCC_END     = 0x400213FF,
    // RCC reset value
    RCC_RESET   = 0x00000000,

} stm32g4x_rcc_reg;

/** Enumeration for GPIO Port A registers */
typedef enum {
    // start address for GPIO port A
    GPIOA_START = 0x48000000,
    // end address for GPIO port A
    GPIOA_END   = 0x480003FF,

    // GPIO port mode register
    GPIOA_MODER_RESET = 0xABFFFFFF,
} stm32g4x_gpioa_reg;

/** Enumeration for GPIO Port B registers */
typedef enum {
    // start address for GPIO port B
    GPIOB_START = 0x48000400,
    // end address for GPIO port B
    GPIOB_END   = 0x480007FF,

    // GPIOB mode start address
    GPIOB_MODER = GPIOB_START + 0x00,

    GPIOB_ODR = GPIOB_START + 0x14,

    // bits for GPIO B Pin 4 & 8
    GPIOB_P4 = 6,
    GPIOB_P8 = 16,

    // GPIO Bit Set/Reset Register 
    GPIOB_BSRR      = GPIOB_START + OFFSET_GPIO_BSRR,
    P4_HI          = 13,
    P4_LO          = P4_HI + OFFSET_GPIO_BSRR,

    // GPIO BSRR reset
    GPIOB_BSRR_RESET = 0x00000000,

    // GPIO port mode register
    GPIOB_MODER_RESET = 0xFFFFFEBF,

} stm32g4x_gpiob_reg;

/** Enumeration for GPIO Port C registers */
typedef enum {
    // start address for GPIO port C
    GPIOC_START     = 0x48000800,
    // Configure Register High
    GPIOC_CRH       = GPIOC_START + OFFSET_GPIO_CRH,

    // GPIO port mode register
    GPIOC_MODER_RESET =  0xFFFFFFFF,

    // bits for GPIO C Pin 13-15
    GPIOC_P13       = 20,   // 20-23
    GPIOC_P14       = 24,   // 24-27
    GPIOC_P15       = 28,   // 28-31
    
    // CRH Reset
    GPIOC_CRH_RESET = 0x44444444,

    // GPIO Bit Set/Reset Register 
    GPIOC_BSRR      = GPIOC_START + OFFSET_GPIO_BSRR,
    P13_HI          = 13,
    P14_HI          = 14,
    P15_HI          = 15,
    P13_LO          = P13_HI + OFFSET_GPIO_BSRR,
    P14_LO          = P14_HI + OFFSET_GPIO_BSRR,
    P15_LO          = P15_HI + OFFSET_GPIO_BSRR,

    // GPIO BSRR reset
    GPIOC_BSRR_RESET = 0x00000000,

    // end address for GPIO port C
    GPIOC_END       = 0x48000BFF,

} stm32g4x_gpioc_reg;

/** Enumeration for GPIO Port D registers */
typedef enum {
    // start address for GPIO port D
    GPIOD_START = 0x40011400,
    // end address for GPIO port D
    GPIOD_END   = 0x400117FF,

} stm32g4x_gpiod_reg;

/** Enumeration for GPIO Port E registers */
typedef enum {
    // start address for GPIO port E
    GPIOE_START = 0x40011800,
    // end address for GPIO port E
    GPIOE_END   = 0x40011BFF,

} stm32g4x_gpioe_reg;

/** Enumeration for GPIO Port F registers */
typedef enum {
    // start address for GPIO port F
    GPIOF_START = 0x40011C00,
    // end address for GPIO port F
    GPIOF_END   = 0x40011FFF,

} stm32g4x_gpiof_reg;

/** Enumeration for GPIO Port G registers */
typedef enum {
    // start address for GPIO port G
    GPIOG_START = 0x40012000,
    // end address for GPIO port G
    GPIOG_END   = 0x400123FF,

} stm32g4x_gpiog_reg;

/** Enumeration for I2C registers */
typedef enum {
    I2C1_START  = 0x40005400,
    I2C1_END    = 0x400057FF,
    I2C2_START  = 0x40005800,
    I2C2_END    = 0x40005BFF,
} stm32g4x_i2c_reg;

#endif
