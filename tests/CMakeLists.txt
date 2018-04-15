set(MODULE_NAME RandomSequences)

find_package(Threads REQUIRED)

include(ExternalProject)
ExternalProject_Add(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
        INSTALL_COMMAND ""
        CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)
#add_dependencies(libgtest src/PseudoRandomSequences.StaticticTests)
add_dependencies(libgtest StatTestProject)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        )

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Set libgmock properties
set_target_properties(libgmock PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        )

include_directories(
#        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/src/PseudoRandomSequences.StaticticTests"
        "${source_dir}/googletest/include"
        "${source_dir}/googlemock/include")

#find_program( GCOV_PATH gcov )
#add_library(gcov IMPORTED STATIC GLOBAL)

set(TEST_SOURCE
    tests.cpp
    )
add_executable(${MODULE_NAME}_test ${TEST_SOURCE})

target_link_libraries(
        ${MODULE_NAME}_test
        libgmock
        libgtest
#        gcov
)

install(TARGETS ${MODULE_NAME}_test DESTINATION bin)

add_test(NAME ${MODULE_NAME}_test COMMAND ${MODULE_NAME}_test)
