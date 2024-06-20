compile_F1:
	arm-none-eabi-gcc -ggdb -Wall relay_test.c -T linker_script.ld -o fan_control.elf -mcpu=cortex-m3 -mthumb -nostdlib
	arm-none-eabi-objcopy -O binary fan_control.elf fan_control.bin

compile_G1:
	arm-none-eabi-gcc -ggdb -Wall relay_test.c -T linker_script.ld -o fan_control.elf -mcpu=cortex-m4 -mthumb -nostdlib
	arm-none-eabi-objcopy -O binary fan_control.elf fan_control.bin

flash_H7:
	st-flash --serial 0030004A3532510F31333430 write fan_control.bin 0x08000000

flash_F1:
	st-flash --serial 18006000010000543931574E write fan_control.bin 0x08000000

flash_G4:
	st-flash --serial 003E00293532510331333430 write fan_control.bin 0x08000000

clean:
	rm -f *.bin *.hex *.elf

