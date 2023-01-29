#ifndef NUTRIENT
#define NUTRIENT

class Nutrient {
 public:
  Nutrient();
  ~Nutrient();

  void recordData();

  double getVolume();
  void setVolume(const double);

  void addToVolume(const double);
  void subtractFromVolume(const double);

 private:
  double volume_;
  double toFlux_;
};

#endif
