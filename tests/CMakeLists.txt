cmake_minimum_required(VERSION 3.10)

# Find GoogleTest package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test source files
set(TEST_SOURCES
  test_serialize.cpp
)

# List of source files required for the `serialize` function
set(SRC_NET
  ../sysd/network/proto.c
  ../sysd/network/proto_check.c
  ../sysd/network/proto_queue.c
  ../sysd/network/proto_pubsub.c
)

set(SRC_SYSD
  ../sysd/system.c
  ../sysd/utils.c
)

# Create test executable
add_executable(sysd_unit_tests ${TEST_SOURCES} ${SRC_NET} ${SRC_SYSD})
target_link_libraries(sysd_unit_tests ${GTEST_LIBRARIES} pthread)

# Add test target
add_test(NAME sysd_unit_tests COMMAND sysd_unit_tests)

