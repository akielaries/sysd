CC		= gcc
CFLGS	= -g -Wall -pedantic -Wparentheses -Wextra -Wundef -Wfloat-equal -Wcast-qual -Wswitch-enum -Wunreachable-code -Wshadow
LLCD	= -llcd

all:
	mkdir -p bin
	${CC} ${CFLGS} sysd/*.c ${LLCD} -o bin/sysd

#sub:
#	${CC} ${CFLGS} sub.c sysd/net.c -o sysdsub

# valgrind binary for checking memory leaks
VG		= valgrind
# valgrind flags
VGFLGS	= --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck

check-mem:
	${VG} ${VGFLGS} ./pub

clean:
	rm -rf bin
