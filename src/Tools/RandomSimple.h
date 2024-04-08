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

/*
 * Written by John D. Cook
 * http://www.johndcook.com
 * https://www.codeproject.com/Articles/25172/Simple-Random-Number-Generation
 */
class RandomSimple {
 public:
  explicit RandomSimple(std::uint32_t seed = 0);

  void reset();

  std::uint32_t getSeed() const;
  void setSeed(std::uint32_t, std::uint32_t);
  void setSeed(std::uint32_t);

  double getUniform();
  std::uint32_t getUniformInt();
  std::uint32_t getUniformInt(std::uint32_t);
  std::uint32_t getUniformInt(std::uint32_t, std::uint32_t);
  double getNormal();
  double getNormal(double, double);
  double getExponential();
  double getExponential(double);
  double getGamma(double, double);
  double getChiSquare(double);
  double getInverseGamma(double, double);
  double getWeibull(double, double);
  double getCauchy(double, double);
  double getLaplace(double, double);
  double getLogNormal(double, double);
  double getBeta(double, double);

 private:
  std::uint32_t seed_;
  std::uint32_t w_;
  std::uint32_t z_;
  double twoPi_;
};

#endif
