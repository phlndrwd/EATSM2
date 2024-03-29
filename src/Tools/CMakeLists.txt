###############################################################################
# Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    #
#                                                                             #
# (C) Copyright 2024, Phil Underwood. All rights reserved.                    #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

list(APPEND EATSM2_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/Date.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Date.h
  ${CMAKE_CURRENT_LIST_DIR}/RandomSimple.cpp
  ${CMAKE_CURRENT_LIST_DIR}/RandomSimple.h
  ${CMAKE_CURRENT_LIST_DIR}/Strings.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Strings.h
  ${CMAKE_CURRENT_LIST_DIR}/Timer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Timer.h
)
