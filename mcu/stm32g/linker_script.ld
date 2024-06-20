MEMORY
{
    /* ARM cortex m4 has 128kb flash and 32kb SRAM */
	
    FLASH (rx): ORIGIN = 0x08000000, LENGTH = 128K
	RAM (rwx): ORIGIN = 0x20000000, LENGTH = 32K
    
    /* ARM cortex m3 has 64kb flash and  20kb SRAM */
    
    /*
    FLASH (rx): ORIGIN = 0x08000000, LENGTH = 64K
    RAM (rwx): ORIGIN = 0x20000000, LENGTH = 20K
    */
}

SECTIONS
{
    .text : {
        . = 0; /* text section starts at this address offset. FLASH + 0 */

        /* At the very beginning, we need the interrupt vectors.
        * We need to mark this KEEP to make sure it doesn't get garbage collected.
        * The syntax *(.foo) means take the .foo sections from all files. */
        KEEP(*(.interrupt_vectors))

        /* The whole interrupt table is 472 bytes long. Advance to that position
         * in case the supplied table was shorter. 
         *
         * - interrupt table is assumed to be 472 bytes with 4 bytes in between 
         *   each item
         *      - see 14.3 Interrupt and exception vectors table 97
         *
         * - interrupt table ends at addres 0x0000_01D4 (468). 4 bytes in
         *   between each table item starting at 0x000_0000 = 468 + 4 bytes 
         *   total = 472
         */
        . = 472; /* ensures code is executed after the interrupt table */

        /* And here comes the rest of the code, ie. all symbols starting with .text */
        *(.text*)
    } > FLASH = 0xFF /* Put this in the flash memory region, default to 0xFF */

    _end_of_ram = ORIGIN(RAM) + LENGTH(RAM); /* Define a symbol for the end of ram */
}

