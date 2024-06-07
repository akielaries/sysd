all:
	arm-none-eabi-gcc -ggdb -Wall relay_test.c -T linker_script.ld -o blink.elf -mcpu=cortex-m3 -mthumb -nostdlib
	arm-none-eabi-objcopy -O binary blink.elf blink.bin

flash_H7:
	st-flash --serial 0030004A3532510F31333430 write blink.bin 0x08000000

flash_F1:
	st-flash --serial 18006000010000543931574E write blink.bin 0x08000000

clean:
	rm -f *.bin *.hex *.elf

