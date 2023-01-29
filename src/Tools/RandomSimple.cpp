#include "RandomSimple.h"

#include <assert.h>
#include <math.h>

#include <random>

#include "Parameters.h"

RandomSimple::RandomSimple() {
  Reset();
  SetSeed(Parameters::Get()->GetRandomSeed());
  mTwoPi = 2.0 * M_PI;
}

RandomSimple::~RandomSimple() {}

void RandomSimple::Reset() {
  mW = 521288629;
  mZ = 362436069;
}

unsigned RandomSimple::GetSeed() const { return mSeed; }

void RandomSimple::SetSeed(unsigned u, unsigned v) {
  if (u != 0) mW = u;
  if (v != 0) mZ = v;
}

void RandomSimple::SetSeed(unsigned seed) {
  mSeed = seed;
  if (mSeed == 0) {
    std::random_device randomDevice;
    mSeed = randomDevice();
  }
  mW = mSeed;
}

double RandomSimple::GetUniform() {
  unsigned u = GetUniformInt();
  return (u + 1.0) * 2.328306435454494e-10;
}

unsigned RandomSimple::GetUniformInt() {
  mZ = 36969 * (mZ & 65535) + (mZ >> 16);
  mW = 18000 * (mW & 65535) + (mW >> 16);
  return (mZ << 16) + mW;
}

unsigned RandomSimple::GetUniformInt(unsigned maximum) { return GetUniformInt() % (maximum + 1); }

double RandomSimple::GetNormal() {
  double u1 = GetUniform();
  double u2 = GetUniform();
  double r = sqrt(-2.0 * log(u1));
  return r * sin(mTwoPi * u2);
}

double RandomSimple::GetNormal(double mean, double standardDeviation) {
  assert(standardDeviation > 0.);
  return mean + standardDeviation * GetNormal();
}

double RandomSimple::GetExponential() { return -log(GetUniform()); }

double RandomSimple::GetExponential(double mean) {
  assert(mean > 0);
  return mean * GetExponential();
}

double RandomSimple::GetGamma(double shape, double scale) {
  double d, c, x, xsquared, v, u;
  assert(shape > 0);
  if (shape >= 1.0) {
    d = shape - 1.0 / 3.0;
    c = 1.0 / sqrt(9.0 * d);
    for (;;) {
      do {
        x = GetNormal();
        v = 1.0 + c * x;
      } while (v <= 0.0);
      v = v * v * v;
      u = GetUniform();
      xsquared = x * x;
      if (u < 1.0 - .0331 * xsquared * xsquared || log(u) < 0.5 * xsquared + d * (1.0 - v + log(v)))
        return scale * d * v;
    }
  } else {
    double g = GetGamma(shape + 1.0, 1.0);
    double w = GetUniform();
    return scale * g * pow(w, 1.0 / shape);
  }
}

double RandomSimple::GetChiSquare(double degreesOfFreedom) { return GetGamma(0.5 * degreesOfFreedom, 2.0); }

double RandomSimple::GetInverseGamma(double shape, double scale) { return 1.0 / GetGamma(shape, 1.0 / scale); }

double RandomSimple::GetWeibull(double shape, double scale) {
  assert(shape > 0.);
  assert(scale > 0);

  return scale * pow(-log(GetUniform()), 1.0 / shape);
}

double RandomSimple::GetCauchy(double median, double scale) {
  assert(scale > 0);
  double p = GetUniform();
  return median + scale * tan(M_PI * (p - 0.5));
}

double RandomSimple::GetLaplace(double mean, double scale) {
  double u = GetUniform();
  return (u < 0.5) ? mean + scale * log(2.0 * u) : mean - scale * log(2 * (1 - u));
}

double RandomSimple::GetLogNormal(double mu, double sigma) { return exp(GetNormal(mu, sigma)); }

double RandomSimple::GetBeta(double a, double b) {
  assert(a > 0);
  assert(b > 0);

  double u = GetGamma(a, 1.0);
  double v = GetGamma(b, 1.0);
  return u / (u + v);
}
