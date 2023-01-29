#ifndef NUTRIENT
#define NUTRIENT

class Nutrient {
 public:
  Nutrient();
  ~Nutrient();

  void RecordData();

  double GetVolume();
  void SetVolume(const double);

  void AddToVolume(const double);
  void SubtractFromVolume(const double);

 private:
  double mVolume;
  double mToFlux;
};

#endif
