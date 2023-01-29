#ifndef TIMESTEP
#define TIMESTEP

class TimeStep {
 public:
  TimeStep();
  ~TimeStep();

  unsigned GetTimeStep() const;

  bool DoRecordData() const;
  void IncrementTimeStep();

 private:
  const unsigned mSamplingRate;

  unsigned mTimeStep;
};

#endif
