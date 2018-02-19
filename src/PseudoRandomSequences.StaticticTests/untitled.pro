QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    maincrypto.cpp \
    bookStackTest/bs.cpp \
    statTests/src/approximateEntropy.cpp \
    statTests/src/blockFrequency.cpp \
    statTests/src/cephes.cpp \
    statTests/src/cusum.cpp \
    statTests/src/dfft.cpp \
    statTests/src/discreteFourierTransform.cpp \
    statTests/src/frequency.cpp \
    statTests/src/generators.cpp \
    statTests/src/linearComplexity.cpp \
    statTests/src/longestRunOfOnes.cpp \
    statTests/src/matrix.cpp \
    statTests/src/nonOverlappingTemplateMatchings.cpp \
    statTests/src/overlappingTemplateMatchings.cpp \
    statTests/src/randomExcursions.cpp \
    statTests/src/randomExcursionsVariant.cpp \
    statTests/src/rank.cpp \
    statTests/src/runs.cpp \
    statTests/src/serial.cpp \
    statTests/src/universal.cpp \
    statTests/src/utilities.cpp \
    maincrypto.cpp \
    statTests/other/genutils.c

HEADERS += \
    book_stack_test.h \
    discrete_fourier_transform_test.h \
    pseudoRandomSequences.h \
    statisticChiSquared.h \
    statTests/include/assess.h \
    statTests/include/cephes.h \
    statTests/include/config.h \
    statTests/include/decls.h \
    statTests/include/defs.h \
    statTests/include/externs.h \
    statTests/include/generators.h \
    statTests/include/genutils.h \
    statTests/include/matrix.h \
    statTests/include/stat_fncs.h \
    statTests/include/utilities.h \
    book_stack_test.h \
    discrete_fourier_transform_test.h \
    pseudoRandomSequences.h \
    statisticChiSquared.h
