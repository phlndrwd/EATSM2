#include "Timer.h"
#include "Parameters.h"
#include "Constants.h"

Timer::Timer( bool start ):
mRunTimeInSeconds( Parameters::Get( )->GetRunTimeInSeconds( ) ) {
    if( start == true ) Start( );
}

Timer::~Timer( ) {
}

void Timer::Start( ) {
    mStartTime = std::chrono::high_resolution_clock::now( );
    mSplitTime = mStartTime;
}

double Timer::Elapsed( ) {
    mElapsedTime = std::chrono::high_resolution_clock::now( );
    std::chrono::duration< double > elapsed;

    elapsed = mElapsedTime - mStartTime;

    return elapsed.count( );
}

double Timer::Split( ) {
    std::chrono::high_resolution_clock::time_point timeNow = std::chrono::high_resolution_clock::now( );
    std::chrono::duration< double > split;

    split = timeNow - mSplitTime;
    mSplitTime = timeNow;

    return split.count( );
}

double Timer::Stop( ) {
    mStopTime = std::chrono::high_resolution_clock::now( );
    std::chrono::duration< double > total;

    total = mStopTime - mStartTime;

    return total.count( );
}

std::string Timer::RemainingString( ) {
    unsigned secondsRemaining = mRunTimeInSeconds - Elapsed( );
    unsigned minutesRemaining = secondsRemaining / Constants::cSecondsInAMinute;
    secondsRemaining = secondsRemaining - ( minutesRemaining * Constants::cSecondsInAMinute );
    unsigned hoursRemaining = minutesRemaining / Constants::cMinutesInAnHour;
    minutesRemaining = minutesRemaining - ( hoursRemaining * Constants::cMinutesInAnHour );
    unsigned daysRemaining = hoursRemaining / Constants::cHoursInADay;
    hoursRemaining = hoursRemaining - ( daysRemaining * Constants::cHoursInADay );

    std::string str = "";
    if( daysRemaining > 0 ) str.append( std::to_string( daysRemaining ) ).append( "d" );
    str.append( std::to_string( hoursRemaining ) ).append( "h" );
    str.append( std::to_string( minutesRemaining ) ).append( "m" );
    str.append( std::to_string( secondsRemaining ) ).append( "s" );

    return str;
}