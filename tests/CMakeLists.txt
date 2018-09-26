set(MODULE_NAME PseudoRandomSequncesApp)

include_directories("${CMAKE_SOURCE_DIR}/tests")
include_directories("${CMAKE_SOURCE_DIR}/src")
#include_directories("${CMAKE_SOURCE_DIR}/src/statistical_tests")
#include_directories("${CMAKE_SOURCE_DIR}/src/nist_statistical_tests")

set(TEST_SOURCE
    tests.cpp
    )

add_executable(${MODULE_NAME}_tests ${TEST_SOURCE})
target_link_libraries(${MODULE_NAME}_tests gtest gmock_main)

set_property(TARGET ${MODULE_NAME}_tests PROPERTY CXX_STANDARD 14)
set_property(TARGET ${MODULE_NAME}_tests PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(NAME ${MODULE_NAME}_tests COMMAND ${MODULE_NAME}_tests)
