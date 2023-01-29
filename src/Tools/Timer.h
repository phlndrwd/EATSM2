#ifndef TIMER
#define TIMER

#include <chrono>
#include <string>

class Timer {
 public:
  Timer(bool start = true);
  ~Timer();

  void Start();
  double Split();
  double Elapsed();
  double Stop();

  std::string RemainingString();

 private:
  const unsigned mRunTimeInSeconds;

  std::chrono::high_resolution_clock::time_point mStartTime;
  std::chrono::high_resolution_clock::time_point mSplitTime;
  std::chrono::high_resolution_clock::time_point mElapsedTime;
  std::chrono::high_resolution_clock::time_point mStopTime;
};

#endif
