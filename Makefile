CC		= gcc
CFLGS	= -std=c99 -g -Wall -pedantic -Wparentheses -Wextra -Wundef -Wfloat-equal -Wcast-qual -Wswitch-enum -Wunreachable-code -Wshadow
LLCD	= -llcd

all:
	mkdir -p bin
	${CC} ${CFLGS} sysd/*.c ${LLCD} -o bin/sysd

#sub:
	#${CC} ${CFLGS} sub.c sysd/net.c -o sysdsub

# valgrind binary for checking memory leaks
VG		= valgrind
# valgrind flags
VGFLGS	= --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck

check-mem:
	${VG} ${VGFLGS} ./pub

clang-analyze:
	find sysd -name "*.c" -exec clang --analyze -Xclang -analyzer-output=text {} \;

clang-tidy:
	clang-tidy sysd/*.c -extra-arg=-std=c99

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem sysd/*.c

clean:
	rm -rf bin *.log *.plist
