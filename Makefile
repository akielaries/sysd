all:
	gcc -g -o pub info.c pub.c libnet/net.c -llcd
	gcc -g -o sub sub.c libnet/net.c

# valgrind binary for checking memory leaks
VG		= valgrind
# valgrind flags
VGFLGS	= --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck

check-mem:
	${VG} ${VGFLGS} ./pub

clean:
	rm pub sub
