#ifndef RANDOMSIMPLE
#define RANDOMSIMPLE

/*
 * Written by John D. Cook
 * http://www.johndcook.com
 * https://www.codeproject.com/Articles/25172/Simple-Random-Number-Generation
 */
class RandomSimple {
 public:
  RandomSimple(unsigned seed = 0);

  void reset();

  unsigned getSeed() const;
  void setSeed(unsigned, unsigned);
  void setSeed(unsigned);

  double getUniform();
  unsigned getUniformInt();
  unsigned getUniformInt(unsigned);
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
  unsigned seed_;
  unsigned w_;
  unsigned z_;
  double twoPi_;
};

#endif
