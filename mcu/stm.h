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
#define 
#define UART1 0x40013800
#define UART2 0x40004400
#define UART3 0x40004800

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
#define OFST_GPIO_CRH       0x04
#define OFST_GPIO_BSSR      0x10

/** Enumeration for GPIO Port C registers */
typedef enum {
    // start address for GPIO port C
    GPIOC_START     = 0x40011000,
    // Configure Register High
    GPIOC_CRH       = GPIOC_START + OFST_GPIO_CRH,

    // bits for GPIO C Pin 13-15
    GPIOC_P13       = 20,   // 20-23
    GPIOC_P14       = 24,   // 24-27
    GPIOC_P15       = 28,   // 28-31
    
    // CRH Reset
    GPIOC_CRH_RESET = 0x44444444,

    // GPIO Bit Set/Reset Register 
    GPIOC_BSSR      = GPIOC_START + OFST_GPIO_BSSR,
    P13_HI          = 13,
    P14_HI          = 14,
    P15_HI          = 15,
    P13_LO          = P13_HI + OFST_GPIO_BSSR,
    P14_LO          = P14_HI + OFST_GPIO_BSSR,
    P15_LO          = P15_HI + OFST_GPIO_BSSR,

    // GPIO BSRR reset
    GPIOC_BSSR_RESET = 0x00000000,

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

/** Enumeration for I2C registers */
typedef enum {
    I2C1_START  = 0x40005400,
    I2C1_END    = 0x400057FF,
    I2C2_START  = 0x40005800,
    I2C2_END    = 0x40005BFF,



} stm32f103x_i2c_reg;

#endif
