#ifndef INITIALSTATE
#define INITIALSTATE

#include <string>
#include <vector>

class Heterotroph;

class InitialState {
 public:
  ~InitialState();
  static InitialState* Get();

  bool Initialise(const std::vector<std::vector<std::string>>&);
  bool isInitialised();

  double& getNutrientVolume();
  double& getAutotrophVolume();
  std::vector<std::vector<Heterotroph*>>& getHeterotrophs();
  unsigned& getInitialPopulationSize();

 private:
  InitialState();
  static InitialState* this_;

  std::vector<std::vector<Heterotroph*>> heterotrophs_;

  bool isInitialised_;
  double nutrientVolume_;
  double autotrophVolume_;
  unsigned initialPopulationSize_;
};

#endif
