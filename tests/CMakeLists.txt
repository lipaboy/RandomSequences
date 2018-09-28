set(MODULE_NAME PseudoRandomSequncesApp)

include_directories("${CMAKE_SOURCE_DIR}/tests")
include_directories("${CMAKE_SOURCE_DIR}/src")

set(TEST_SOURCE
    tests.cpp
    )

	# Boost library linking

if(WIN32)
		add_definitions( -DBOOST_ALL_DYN_LINK )  #If not VS will give linking errors of redefinitions
		set(Boost_USE_STATIC_LIBS OFF )
		set(Boost_USE_MULTITHREADED ON)
		set(Boost_USE_STATIC_RUNTIME OFF)
        find_package(Boost COMPONENTS thread chrono regex REQUIRED )

        include_directories(${Boost_INCLUDE_DIRS})
endif(WIN32)

add_executable(${MODULE_NAME}_tests ${TEST_SOURCE})

target_link_libraries(${MODULE_NAME}_tests 
	PSEUDO_RANDOM_SEQUENCES_LIB
	NIST_Statistical_tests_LIB
	gtest 
	gmock_main)

if(WIN32)
        target_link_libraries( ${MODULE_NAME}_tests  ${Boost_LIBRARIES} )
endif(WIN32)

set_property(TARGET ${MODULE_NAME}_tests PROPERTY CXX_STANDARD 14)
set_property(TARGET ${MODULE_NAME}_tests PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(NAME ${MODULE_NAME}_tests COMMAND ${MODULE_NAME}_tests)
