###############################################################################
# Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    #
#                                                                             #
# (C) Copyright 2024, Phil Underwood. All rights reserved.                    #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

cmake_minimum_required(VERSION 3.25)

project(eatsm2 LANGUAGES CXX)

set(JINO_PATH "../jino/")

file(COPY ${CMAKE_SOURCE_DIR}/input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Download required files
include(FetchContent)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/external/")
FetchContent_Declare(cpplint
  GIT_REPOSITORY "https://github.com/cpplint/cpplint.git"
  GIT_TAG "2.0.0"
)
FetchContent_MakeAvailable(cpplint)

list(APPEND EATSM2_SOURCES
  src/Data/Enums.h
  src/Data/MatrixDatum.cpp
  src/Data/MatrixDatum.h
  src/Data/Structs.h
  src/Data/VectorDatum.cpp
  src/Data/VectorDatum.h

  src/Input/Constants.h
  src/Input/Parameters.cpp
  src/Input/Parameters.h

  src/Model/Autotrophs.cpp
  src/Model/Autotrophs.h
  src/Model/EcologicalData.cpp
  src/Model/EcologicalData.h
  src/Model/EcologicalFunctions.cpp
  src/Model/EcologicalFunctions.h
  src/Model/EncounterAlgorithm.cpp
  src/Model/EncounterAlgorithm.h
  src/Model/Environment.h
  src/Model/Environment.cpp
  src/Model/Heterotroph.cpp
  src/Model/Heterotroph.h
  src/Model/Heterotrophs.cpp
  src/Model/Heterotrophs.h
  src/Model/Life.cpp
  src/Model/Life.h
  src/Model/Nutrient.cpp
  src/Model/Nutrient.h
  src/Model/SizeClass.cpp
  src/Model/SizeClass.h
  src/Model/TimeStep.cpp
  src/Model/TimeStep.h
  src/Model/Traits.cpp
  src/Model/Traits.h

  src/Tools/Date.cpp
  src/Tools/Date.h
  src/Tools/RandomSimple.cpp
  src/Tools/RandomSimple.h
  src/Tools/Strings.cpp
  src/Tools/Strings.h
  src/Tools/Timer.cpp
  src/Tools/Timer.h
)

add_subdirectory(${JINO_PATH} jino_build)

add_executable(eatsm2 src/Main.cpp ${EATSM2_SOURCES})
target_include_directories(eatsm2 PRIVATE
  src/Data
  src/Input
  src/Model
  src/Tools
)
target_compile_features(eatsm2 PRIVATE cxx_std_23)
target_compile_options(eatsm2 PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(eatsm2 PRIVATE
  jino
)

## Create tests
enable_testing()
add_test(NAME coding_norms
         COMMAND "${FETCHCONTENT_BASE_DIR}/cpplint-src/cpplint.py" --quiet --recursive
                 ${PROJECT_SOURCE_DIR}/src
                 ${PROJECT_SOURCE_DIR}/test)


