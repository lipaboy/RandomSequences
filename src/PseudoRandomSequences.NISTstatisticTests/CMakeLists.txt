cmake_minimum_required(VERSION 2.8)

project(StatRandomTestCmake)

set(SOURCE_FILES

    statTests/src/approximateEntropy.c
        statTests/src/assess.c
        statTests/src/blockFrequency.c
        statTests/src/cephes.c
        statTests/src/cusum.c
        statTests/src/dfft.c
        statTests/src/discreteFourierTransform.c
        statTests/src/frequency.c
        statTests/src/generators.c
        statTests/src/genutils.c
        statTests/src/linearComplexity.c
        statTests/src/longestRunOfOnes.c
        statTests/src/matrix.c
        statTests/src/nonOverlappingTemplateMatchings.c
        statTests/src/overlappingTemplateMatchings.c
        statTests/src/randomExcursions.c
        statTests/src/randomExcursionsVariant.c
        statTests/src/rank.c
        statTests/src/runs.c
        statTests/src/serial.c
        statTests/src/universal.c
        statTests/src/utilities.c

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
#        assess.h

    )

add_executable(${PROJECT_NAME}
    #"main.cpp"
    ${SOURCE_FILES})
