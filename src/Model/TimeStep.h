#ifndef TIMESTEP
#define TIMESTEP

class TimeStep {
 public:
  TimeStep();

  const unsigned& getTimeStep() const;

  bool doRecordData() const;
  void incrementTimeStep();

 private:
  const unsigned samplingRate_;

  unsigned timeStep_;
};

#endif
