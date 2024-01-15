CC		= gcc
CFLGS	= -g -Wall -pedantic -Wparentheses -Wextra -Wundef -Wfloat-equal -Wcast-qual -Wswitch-enum -Wunreachable-code -Wshadow
LLCD	= -llcd

pub:
	${CC} ${CFLGS} pub.c info.c libnet/net.c ${LLCD} -o sysdpub

sub:
	${CC} ${CFLGS} sub.c libnet/net.c -o sysdsub

# valgrind binary for checking memory leaks
VG		= valgrind
# valgrind flags
VGFLGS	= --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck

check-mem:
	${VG} ${VGFLGS} ./pub

clean:
	rm sysdpub sysdsub
