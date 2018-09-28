cmake_minimum_required(VERSION 3.0)

set(MODULE_NAME NIST_Statistical_tests)
project(${MODULE_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# if you increase version to 17 then you can't compile the program with Boost(x64) library on Windows
set(CMAKE_CXX_STANDARD 14)

set(${MODULE_NAME}_LIB_SOURCE_FILES 

        include/cephes.h
        include/config.h
        include/decls.h
        include/defs.h
        include/externs.h
        include/generators.h
        include/genutils.h
        include/matrix.h
        include/stat_fncs.h
        include/utilities.h
#	stat sources
        src/approximateEntropy.c
		#src/assess.c
        src/blockFrequency.c
        src/cephes.c
        src/cusum.c
        src/dfft.c
        src/discreteFourierTransform.c
        src/frequency.c
        src/generators.c
        src/genutils.c
        src/linearComplexity.c
        src/longestRunOfOnes.c
        src/matrix.c
        src/nonOverlappingTemplateMatchings.c
        src/overlappingTemplateMatchings.c
        src/randomExcursions.c
        src/randomExcursionsVariant.c
        src/rank.c
        src/runs.c
        src/serial.c
        src/universal.c
        src/utilities.c
		
)

# Declare the library
add_library(${MODULE_NAME}_LIB  STATIC
    ${${MODULE_NAME}_LIB_SOURCE_FILES}
)

# Specify here the include directories exported
# by this library
target_include_directories(${MODULE_NAME}_LIB PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${MODULE_NAME}_run
	src/assess.c)

target_link_libraries( ${MODULE_NAME}_run ${MODULE_NAME}_LIB )