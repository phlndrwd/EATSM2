#ifndef TIMER
#define TIMER

#include <chrono>
#include <string>

class Timer {
 public:
  explicit Timer(bool goNow = true);
  ~Timer();

  void go();
  double split();
  double elapsed();
  double stop();

  std::string remainingString();

 private:
  const unsigned runTimeInSeconds_;

  std::chrono::high_resolution_clock::time_point startTime_;
  std::chrono::high_resolution_clock::time_point splitTime_;
  std::chrono::high_resolution_clock::time_point elapsedTime_;
  std::chrono::high_resolution_clock::time_point stopTime_;
};

#endif
