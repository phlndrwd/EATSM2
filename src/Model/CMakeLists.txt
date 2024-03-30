###############################################################################
# Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    #
#                                                                             #
# (C) Copyright 2024, Phil Underwood. All rights reserved.                    #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

list(APPEND EATSM2_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/Autotrophs.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Autotrophs.h
  ${CMAKE_CURRENT_LIST_DIR}/EcologicalFunctions.cpp
  ${CMAKE_CURRENT_LIST_DIR}/EcologicalFunctions.h
  ${CMAKE_CURRENT_LIST_DIR}/EncounterAlgorithm.cpp
  ${CMAKE_CURRENT_LIST_DIR}/EncounterAlgorithm.h
  ${CMAKE_CURRENT_LIST_DIR}/Environment.h
  ${CMAKE_CURRENT_LIST_DIR}/Environment.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Environment.h
  ${CMAKE_CURRENT_LIST_DIR}/Heterotroph.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Heterotroph.h
  ${CMAKE_CURRENT_LIST_DIR}/Life.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Life.h
  ${CMAKE_CURRENT_LIST_DIR}/Nutrient.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Nutrient.h
  ${CMAKE_CURRENT_LIST_DIR}/SizeClass.cpp
  ${CMAKE_CURRENT_LIST_DIR}/SizeClass.h
  ${CMAKE_CURRENT_LIST_DIR}/TimeStep.cpp
  ${CMAKE_CURRENT_LIST_DIR}/TimeStep.h
  ${CMAKE_CURRENT_LIST_DIR}/Traits.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Traits.h
)
