cmake_minimum_required(VERSION 3.10)

# Project name and version
project(sysd VERSION 1.0)

# Set the C standard and compiler flags
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -pedantic -Wparentheses -Wextra -Wundef -Wfloat-equal -Wcast-qual -Wswitch-enum -Wunreachable-code -Wshadow")

# Include directories for dependencies
include_directories(${CMAKE_SOURCE_DIR}/include)

# Dependencies: Manually link against cJSON and libinfluxdb
set(LIBS -lcjson -llibinfluxdb)

# Add source files
set(SRC_NET
  sysd/network/proto.c
  sysd/network/proto_check.c
  sysd/network/proto_queue.c
  sysd/network/proto_pubsub.c
)

set(SRC_SYSD
  sysd/system.c
  sysd/utils.c
  sysd/main.c
)

# Combine source files
file(GLOB SYS_SRC ${SRC_NET} ${SRC_SYSD})

# Create the executable
add_executable(sysd ${SYS_SRC})
target_link_libraries(sysd ${LIBS})

# Add custom targets
add_subdirectory(tests)

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

