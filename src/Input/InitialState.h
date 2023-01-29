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
  bool IsInitialised();

  double& GetNutrientVolume();
  double& GetAutotrophVolume();
  std::vector<std::vector<Heterotroph*>>& GetHeterotrophs();
  unsigned& GetInitialPopulationSize();

 private:
  InitialState();
  static InitialState* mThis;

  std::vector<std::vector<Heterotroph*>> mHeterotrophs;

  bool mIsInitialised;
  double mNutrientVolume;
  double mAutotrophVolume;
  unsigned mInitialPopulationSize;
};

#endif
