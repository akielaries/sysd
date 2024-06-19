MEMORY
{
    /* ARM cortex m4 has 128kb flash and 32kb SRAM */
	
    FLASH (rx): ORIGIN = 0x08000000, LENGTH = 128K
	SRAM (rwx): ORIGIN = 0x20000000, LENGTH = 32K
    
    /* ARM cortex m3 has 64kb flash and  20kb SRAM */
    
    /*
    FLASH (rx): ORIGIN = 0x08000000, LENGTH = 64K
    RAM (rwx): ORIGIN = 0x20000000, LENGTH = 20K
    */
}

SECTIONS
{
    .text : {
        /* Start at 0 for the code in flash */
        . = 0;

        /* At the very beginning, we need the interrupt vectors.
        * We need to mark this KEEP to make sure it doesn't get garbage collected.
        * The syntax *(.foo) means take the .foo sections from all files. */
        KEEP(*(.interrupt_vectors))

        /* The whole interrupt table is 304 bytes long. Advance to that position in case
        * the supplied table was shorter. */
        . = 304;

        /* And here comes the rest of the code, ie. all symbols starting with .text */
        *(.text*)
    } > FLASH = 0xFF /* Put this in the flash memory region, default to 0xFF */

    _end_of_ram = ORIGIN(RAM) + LENGTH(RAM); /* Define a symbol for the end of ram */
}

