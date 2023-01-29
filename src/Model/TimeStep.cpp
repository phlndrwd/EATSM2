#include "TimeStep.h"
#include "Parameters.h"

TimeStep::TimeStep()
    : samplingRate_(Parameters::Get()->getSamplingRate()) {
  timeStep_ = 0;
}

TimeStep::~TimeStep() {}

unsigned TimeStep::getTimeStep() const {
  return timeStep_;
}

bool TimeStep::doRecordData() const {
  return (timeStep_ % samplingRate_ == 0);
}

void TimeStep::incrementTimeStep() {
  ++timeStep_;
}
