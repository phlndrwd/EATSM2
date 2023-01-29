#include "RandomSimple.h"

#include <cassert>
#include <cmath>
#include <random>

#include "Parameters.h"

RandomSimple::RandomSimple() {
  reset();
  setSeed(Parameters::Get()->getRandomSeed());
  twoPi_ = 2.0 * M_PI;
}

RandomSimple::~RandomSimple() {}

void RandomSimple::reset() {
  w_ = 521288629;
  z_ = 362436069;
}

unsigned RandomSimple::getSeed() const { return seed_; }

void RandomSimple::setSeed(unsigned u, unsigned v) {
  if (u != 0) w_ = u;
  if (v != 0) z_ = v;
}

void RandomSimple::setSeed(unsigned seed) {
  seed_ = seed;
  if (seed_ == 0) {
    std::random_device randomDevice;
    seed_ = randomDevice();
  }
  w_ = seed_;
}

double RandomSimple::getUniform() {
  unsigned u = getUniformInt();
  return (u + 1.0) * 2.328306435454494e-10;
}

unsigned RandomSimple::getUniformInt() {
  z_ = 36969 * (z_ & 65535) + (z_ >> 16);
  w_ = 18000 * (w_ & 65535) + (w_ >> 16);
  return (z_ << 16) + w_;
}

unsigned RandomSimple::getUniformInt(unsigned maximum) { return getUniformInt() % (maximum + 1); }

double RandomSimple::getNormal() {
  double u1 = getUniform();
  double u2 = getUniform();
  double r = sqrt(-2.0 * log(u1));
  return r * sin(twoPi_ * u2);
}

double RandomSimple::getNormal(double mean, double standardDeviation) {
  assert(standardDeviation > 0.);
  return mean + standardDeviation * getNormal();
}

double RandomSimple::getExponential() {
  return -log(getUniform());
}

double RandomSimple::getExponential(double mean) {
  assert(mean > 0);
  return mean * getExponential();
}

double RandomSimple::getGamma(double shape, double scale) {
  double d, c, x, xsquared, v, u;
  assert(shape > 0);
  if (shape >= 1.0) {
    d = shape - 1.0 / 3.0;
    c = 1.0 / sqrt(9.0 * d);
    for (;;) {
      do {
        x = getNormal();
        v = 1.0 + c * x;
      } while (v <= 0.0);
      v = v * v * v;
      u = getUniform();
      xsquared = x * x;
      if (u < 1.0 - .0331 * xsquared * xsquared || log(u) < 0.5 * xsquared + d * (1.0 - v + log(v)))
        return scale * d * v;
    }
  } else {
    double g = getGamma(shape + 1.0, 1.0);
    double w = getUniform();
    return scale * g * pow(w, 1.0 / shape);
  }
}

double RandomSimple::getChiSquare(double degreesOfFreedom) {
  return getGamma(0.5 * degreesOfFreedom, 2.0);
}

double RandomSimple::getInverseGamma(double shape, double scale) {
  return 1.0 / getGamma(shape, 1.0 / scale);
}

double RandomSimple::getWeibull(double shape, double scale) {
  assert(shape > 0.);
  assert(scale > 0);

  return scale * pow(-log(getUniform()), 1.0 / shape);
}

double RandomSimple::getCauchy(double median, double scale) {
  assert(scale > 0);
  double p = getUniform();
  return median + scale * tan(M_PI * (p - 0.5));
}

double RandomSimple::getLaplace(double mean, double scale) {
  double u = getUniform();
  return (u < 0.5) ? mean + scale * log(2.0 * u) : mean - scale * log(2 * (1 - u));
}

double RandomSimple::getLogNormal(double mu, double sigma) {
  return exp(getNormal(mu, sigma));
}

double RandomSimple::getBeta(double a, double b) {
  assert(a > 0);
  assert(b > 0);

  double u = getGamma(a, 1.0);
  double v = getGamma(b, 1.0);
  return u / (u + v);
}
