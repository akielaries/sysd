cmake_minimum_required(VERSION 3.10)

# Project name and version
project(sysd VERSION 1.0)

# Set the C standard and compiler flags
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -pedantic -Wparentheses -Wextra -Wundef -Wfloat-equal -Wcast-qual -Wswitch-enum -Wunreachable-code -Wshadow")

# Check for the liblcd header file
find_file(LIBLCD_HEADER liblcd/liblcd.h)

if(LIBLCD_HEADER)
    message(STATUS "Found liblcd header: ${LIBLCD_HEADER}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__LIBLCD__")
    set(LLCD -llcd)
else()
    message(WARNING "liblcd header not found. Skipping linking with liblcd.")
    set(LLCD "")
endif()

# Check if the system is 32-bit or 64-bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Found 64-bit system")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "Found 32-bit system")
else()
    message(WARNING "Unknown pointer size: ${CMAKE_SIZEOF_VOID_P}")
endif()

# Specify the source files
file(GLOB SYS_SRC "sysd/*.c")

# Create the executable
add_executable(sysd ${SYS_SRC})
target_link_libraries(sysd ${LLCD})

# Custom target for valgrind memory check
add_custom_target(check-mem
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck $<TARGET_FILE:sysd>
    DEPENDS sysd
)

# Custom target for clang-tidy
add_custom_target(clang-tidy
    COMMAND clang-tidy ${SYS_SRC} -extra-arg=-std=c99
    DEPENDS sysd
)

# Custom target for cppcheck
add_custom_target(cppcheck
    COMMAND cppcheck --enable=all --suppress=missingIncludeSystem ${SYS_SRC}
)

# Custom target for libfuzzer
add_custom_target(libfuzzer
    COMMAND clang++ -g -fprofile-instr-generate -fcoverage-mapping -fsanitize=address,fuzzer sysd/config.c sysdfuzzer.cc -o fuzzsysd
)

# Custom target for osv-scanner
add_custom_target(osv
    COMMAND osv-scanner -r .
)

# Installation rules
install(TARGETS sysd DESTINATION /usr/local/bin)
install(FILES sysd.conf DESTINATION /etc)
install(FILES sysd.service DESTINATION /etc/systemd)

# Uninstallation rules
add_custom_target(uninstall
    COMMAND rm /usr/local/bin/sysd
    COMMAND rm /etc/sysd.conf
    COMMAND rm /etc/systemd/sysd.service
)

# Clean target
set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "crash-*" "*.log" "*.plist" "*.out" "*.profraw" "fuzzsysd")

