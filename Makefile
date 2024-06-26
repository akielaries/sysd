CC		= gcc
CFLGS	= -std=c99 -g -Wall -pedantic -Wparentheses \
		  -Wextra -Wundef -Wfloat-equal -Wcast-qual \
		  -Wswitch-enum -Wunreachable-code -Wshadow #-fanalyzer
LLCD	= -llcd

all:
	mkdir -p bin
	${CC} ${CFLGS} sysd/*.c ${LLCD} -o bin/sysd

# valgrind binary for checking memory leaks
VG		= valgrind
# valgrind flags
VGFLGS	= --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck

check-mem:
	${VG} ${VGFLGS} ./bin/sysd

clang-tidy:
	clang-tidy sysd/*.c -extra-arg=-std=c99

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem sysd/*.c

libfuzzer:
	clang++ -g -fprofile-instr-generate -fcoverage-mapping -fsanitize=address,fuzzer sysd/config.c sysdfuzzer.cc -o fuzzsysd

osv:
	osv-scanner -r .

install:
	cp bin/sysd /usr/local/bin/
	cp sysd.conf /etc/
	cp sysd.service /etc/systemd/
	# systemctl start /etc/systemd/sysd.service

uninstall:
	rm /usr/local/bin/sysd
	rm /etc/sysd.conf
	rm /etc/systemd/sysd.service

clean:
	rm -rf crash-* bin *.log *.plist *.out *.profraw fuzzsysd build
