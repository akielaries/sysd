MEMORY
{
    /* ARM cortex m3 has 64kb flash and  20kb SRAM */
    
    FLASH (rx): ORIGIN = 0x08000000, LENGTH = 64K
    RAM (rwx): ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
    .text : {
        /* Start at 0 for the code in flash. this is page 0 in main memory 
         * 3.3.3 Embedded Flash memory
         */
        . = 0; /* text section starts at this address offset. FLASH + 0 */

        /* At the very beginning, we need the interrupt vectors.
        * We need to mark this KEEP to make sure it doesn't get garbage collected.
        * The syntax *(.foo) means take the .foo sections from all files. */
        KEEP(*(.interrupt_vectors))

        /* The whole interrupt table is 304 bytes long. Advance to that position
         * in case the supplied table was shorter. 
         *
         * - interrupt table is assumed to be 304 bytes with 4 bytes in between 
         *   each item
         *      - see 10.1.2 Interrupt and exception vectors table 63 for 
         *        non-connectivity line devices (we don't have ethernet...)
         *
         * - interrupt table ends at addres 0x0000_012C (300). 4 bytes in
         *   between each table item starting at 0x000_0000 = 300 + 4 bytes 
         *   total
         */
        . = 304; /* ensures code is executed after the interrupt table */

        /* And here comes the rest of the code, ie. all symbols starting with .text */
        *(.text*)
    } > FLASH = 0xFF /* Put this in the flash memory region, default to 0xFF */

    _end_of_ram = ORIGIN(RAM) + LENGTH(RAM); /* Define a symbol for the end of ram */
}

