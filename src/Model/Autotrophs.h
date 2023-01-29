#ifndef AUTOTROPHS
#define AUTOTROPHS

class Nutrient;

class Autotrophs {
 public:
  Autotrophs(Nutrient&);
  ~Autotrophs();

  void RecordData();

  void Update();

  double GetVolume();
  void SetVolume(const double);

  void AddToVolume(const double);
  void SubtractFromVolume(const double);

 private:
  Nutrient& mNutrient;

  double mVolume;
  double mToFlux;
};

#endif
