###############################################################################
# Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    #
#                                                                             #
# (C) Copyright 2024, Phil Underwood. All rights reserved.                    #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

list(APPEND EATSM2_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/Enums.h
  ${CMAKE_CURRENT_LIST_DIR}/MatrixDatum.cpp
  ${CMAKE_CURRENT_LIST_DIR}/MatrixDatum.h
  ${CMAKE_CURRENT_LIST_DIR}/Structs.h
  ${CMAKE_CURRENT_LIST_DIR}/VectorDatum.cpp
  ${CMAKE_CURRENT_LIST_DIR}/VectorDatum.h
)
