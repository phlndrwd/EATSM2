#include "TimeStep.h"
#include "Parameters.h"

TimeStep::TimeStep( ):
mSamplingRate( Parameters::Get( )->GetSamplingRate( ) ) {
    mTimeStep = 0;
}

TimeStep::~TimeStep( ) {

}

unsigned TimeStep::GetTimeStep( ) const {
    return mTimeStep;
}

bool TimeStep::DoRecordData( ) const {
    return ( mTimeStep % mSamplingRate == 0 );
}

void TimeStep::IncrementTimeStep( ) {
    ++mTimeStep;
}