cmake_minimum_required(VERSION 3.5.1)
project(Random_Sequences_App)

#set (CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_SOURCE_DIR}/src/PseudoRandomSequences.StaticticTests"
        "${source_dir}/googletest/include"
        "${source_dir}/googlemock/include")

include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")
#enable_cxx_compiler_flag_if_supported("-std=c++17")

#set(CMAKE_C_COMPILER gcc-7)
#set(CMAKE_CXX_COMPILER g++-7)

add_subdirectory(tests)
add_subdirectory(src/PseudoRandomSequences.StaticticTests)

