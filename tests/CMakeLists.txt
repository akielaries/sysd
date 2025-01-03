cmake_minimum_required(VERSION 3.10)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(LIBINFLUXDB -linfluxdb)
set(LCJSON -lcjson)

set(TEST_SOURCES
  test_serialize.cpp
)

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

# create executable
add_executable(sysd_unit_tests ${TEST_SOURCES} ${SRC_NET} ${SRC_SYSD})
#target_link_libraries(sysd_unit_tests ${GTEST_LIBRARIES} pthread)
target_link_libraries(sysd_unit_tests PRIVATE ${GTEST_LIBRARIES} pthread ${LIBINFLUXDB} ${LCJSON})

# add test target
add_test(NAME sysd_unit_tests COMMAND sysd_unit_tests)
