cmake_minimum_required(VERSION 3.0)

project(PseudoRandomSequences)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# if you increase version to 17 then you can't compile the program with Boost(x64) library on Windows
set(CMAKE_CXX_STANDARD 14)

set(APP_SOURCES 
		maincrypto.cpp
                generatorsTestConfigRun.cpp
                runStatisticTests.cpp

                tests.cpp

#	lipaboyLib
                lipaboyLibrary/src/maths/fixed_precision_number.h
                lipaboyLibrary/src/maths/precision_number.h
                pseudoRandomSequences.h

#	IStatTest
                i_statistical_test.h
#	order test
                statisticChiSquared.h
                bookStackTest/order_test.h
#	book stack test
                bookStackTest/bs.cpp
#	stat headers
                nist_tests_wrapper.h
                nist_tests_wrapper.cpp

                statTests/include/assess.h
                statTests/include/cephes.h
                statTests/include/config.h
                statTests/include/decls.h
                statTests/include/defs.h
                statTests/include/externs.h
                statTests/include/generators.h
                statTests/include/genutils.h
                statTests/include/matrix.h
                statTests/include/stat_fncs.h
                statTests/include/utilities.h
#	stat sources
                statTests/src/approximateEntropy.cpp
                statTests/src/blockFrequency.cpp
                statTests/src/cephes.cpp
                statTests/src/cusum.cpp
                statTests/src/dfft.cpp
                statTests/src/discreteFourierTransform.cpp
                statTests/src/frequency.cpp
                statTests/src/generators.cpp
                statTests/src/genutils.cpp
                statTests/src/linearComplexity.cpp
                statTests/src/longestRunOfOnes.cpp
                statTests/src/matrix.cpp
                statTests/src/nonOverlappingTemplateMatchings.cpp
                statTests/src/overlappingTemplateMatchings.cpp
                statTests/src/randomExcursions.cpp
                statTests/src/randomExcursionsVariant.cpp
                statTests/src/rank.cpp
                statTests/src/runs.cpp
                statTests/src/serial.cpp
                statTests/src/universal.cpp
                statTests/src/utilities.cpp
)

find_package(OpenMP)
if (OPENMP_FOUND)
		set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if(WIN32)
		add_definitions( -DBOOST_ALL_DYN_LINK )  #If not VS will give linking errors of redefinitions
		set(Boost_USE_STATIC_LIBS OFF )
		set(Boost_USE_MULTITHREADED ON)
		set(Boost_USE_STATIC_RUNTIME OFF)
                find_package(Boost COMPONENTS thread chrono regex REQUIRED )

                INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
endif(WIN32)

if(WIN32)
        add_executable(PseudoRandomSequencesApp_run 
		WIN32 
		${APP_SOURCES})
else(WIN32)
        add_executable(PseudoRandomSequencesApp_run ${APP_SOURCES})
endif(WIN32)

if(WIN32)
                TARGET_LINK_LIBRARIES( PseudoRandomSequencesApp_run ${Boost_LIBRARIES} )

        add_custom_command(
                TARGET PseudoRandomSequencesApp_run POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        $<TARGET_FILE_DIR:PseudoRandomSequencesApp_run>)
endif(WIN32)
