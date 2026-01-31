###############################################################################
# Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)
#
# (C) Copyright 2024, Phil Underwood. All rights reserved.
#
# This software is licensed under the terms of the 3-Clause BSD License
# which can be obtained from https://opensource.org/license/bsd-3-clause/.
###############################################################################

cmake_minimum_required(VERSION 3.25)

project(eatsm2 LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Runtime data
# ---------------------------------------------------------------------------

file(COPY ${CMAKE_SOURCE_DIR}/input
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# ---------------------------------------------------------------------------
# Tooling (cpplint)
# ---------------------------------------------------------------------------

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

FetchContent_Declare(cpplint
  GIT_REPOSITORY https://github.com/cpplint/cpplint.git
  GIT_TAG 2.0.0
)
FetchContent_MakeAvailable(cpplint)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------

add_subdirectory(../jino jino_build)

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------

add_executable(eatsm2
  src/Main.cpp

  src/Input/Parameters.cpp

  src/Model/Autotrophs.cpp
  src/Model/EncounterAlgorithm.cpp
  src/Model/Environment.cpp
  src/Model/Functions.cpp
  src/Model/Heterotroph.cpp
  src/Model/HeterotrophPopulation.cpp
  src/Model/Life.cpp
  src/Model/Nutrient.cpp
  src/Model/SizeClass.cpp
  src/Model/Traits.cpp

  src/Tools/RandomSimple.cpp
)

# Flat-header include policy (intentional)
target_include_directories(eatsm2 PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Input
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Model
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Tools
)

target_compile_features(eatsm2 PRIVATE cxx_std_23)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(eatsm2 PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(eatsm2 PRIVATE jino)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------

enable_testing()

add_test(
  NAME coding_norms
  COMMAND ${FETCHCONTENT_BASE_DIR}/cpplint-src/cpplint.py
          --quiet --recursive
          ${PROJECT_SOURCE_DIR}/src
          ${PROJECT_SOURCE_DIR}/test
)
