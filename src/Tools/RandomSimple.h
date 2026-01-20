/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef RANDOMSIMPLE
#define RANDOMSIMPLE

#include <cstdint>
#include <stdfloat>

/*
 * Written by John D. Cook
 * http://www.johndcook.com
 * https://www.codeproject.com/articles/Simple-Random-Number-Generation#comments-section
 */
class RandomSimple {
 public:
  explicit RandomSimple(std::uint32_t seed = 0);

  void reset();

  std::uint32_t getSeed() const;
  void setSeed(std::uint32_t, std::uint32_t);
  void setSeed(std::uint32_t);

  std::float64_t getUniform();
  std::uint32_t getUniformInt();
  std::uint32_t getUniformInt(std::uint32_t);
  std::uint32_t getUniformInt(std::uint32_t, std::uint32_t);
  std::float64_t getNormal();
  std::float64_t getNormal(std::float64_t, std::float64_t);
  std::float64_t getExponential();
  std::float64_t getExponential(std::float64_t);
  std::float64_t getGamma(std::float64_t, std::float64_t);
  std::float64_t getChiSquare(std::float64_t);
  std::float64_t getInverseGamma(std::float64_t, std::float64_t);
  std::float64_t getWeibull(std::float64_t, std::float64_t);
  std::float64_t getCauchy(std::float64_t, std::float64_t);
  std::float64_t getLaplace(std::float64_t, std::float64_t);
  std::float64_t getLogNormal(std::float64_t, std::float64_t);
  std::float64_t getBeta(std::float64_t, std::float64_t);

 private:
  std::uint32_t seed_;
  std::uint32_t w_;
  std::uint32_t z_;
  std::float64_t twoPi_;
};

#endif
