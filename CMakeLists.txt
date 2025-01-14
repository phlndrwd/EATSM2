###############################################################################
# Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    #
#                                                                             #
# (C) Copyright 2024, Phil Underwood. All rights reserved.                    #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

cmake_minimum_required(VERSION 3.5)

project(EATSM2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(COPY ${CMAKE_SOURCE_DIR}/input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/src/Data)
include_directories(${CMAKE_SOURCE_DIR}/src/Input)
include_directories(${CMAKE_SOURCE_DIR}/src/Model)
include_directories(${CMAKE_SOURCE_DIR}/src/Output)
include_directories(${CMAKE_SOURCE_DIR}/src/Tools)

include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/Data/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/Input/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/Model/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/Output/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/Tools/CMakeLists.txt)

add_compile_options(-Wall -Wextra -Wpedantic -std=c++17)
add_executable(EATSM2 ${EATSM2_SOURCES}
  src/Model/EncounterAlgorithm.h
  src/Model/EncounterAlgorithm.cpp
  src/Model/EcologicalData.h
  src/Model/EcologicalData.cpp
  src/Model/Heterotrophs.h
  src/Model/Heterotrophs.cpp
  src/Data/OutputData.h
  src/Data/OutputData.cpp)

install(TARGETS EATSM2 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

