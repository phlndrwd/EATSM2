#ifndef HETEROTROPHS
#define HETEROTROPHS

#include "HeterotrophProcessor.h"
#include "HeterotrophData.h"
#include "Nutrient.h"
#include "Autotrophs.h"
#include "Timer.h"

struct TestStruct {
    double mVolumeHeritable;
    double mVolumeMinimum;
    double mVolumeReproduction;

    double mVolumeActual;
    double mTrophicLevel;
    
    unsigned mAge;

    bool mHasFed;
    bool mIsDead;
};

class Heterotrophs {
public:
    Heterotrophs( Nutrient&, Autotrophs& );
    ~Heterotrophs( );

    void Update( );
    bool RecordData( );

    Types::HeterotrophPointer GetIndividual( const unsigned, const unsigned );
    std::size_t GetSizeClassPopulation( const unsigned ) const;

private:
    void CreateInitialPopulation( );
    void CalculateFeedingProbabilities( );

    void Feeding( );
    void Metabolisation( );
    void Starvation( );
    void Reproduction( );

    Types::HeterotrophPointer GetRandomIndividualFromSizeClass( const unsigned );
    Types::HeterotrophPointer GetRandomPredatorFromSizeClass( const unsigned );
    Types::HeterotrophPointer GetRandomPreyFromSizeClass( const unsigned, const Types::HeterotrophPointer );

    void FeedFromAutotrophs( Types::HeterotrophPointer );
    void FeedFromHeterotrophs( Types::HeterotrophPointer, const unsigned );

    void StageForMoving( Types::HeterotrophPointer, const unsigned );
    void MoveSizeClass( Types::HeterotrophPointer, const unsigned );
    bool RemoveFromSizeClass( Types::HeterotrophPointer, const unsigned );
    void DeleteIndividual( Types::HeterotrophPointer );

    void Starve( Types::HeterotrophPointer );
    void Kill( Types::HeterotrophPointer );

    void AddChildren( );
    void MoveIndividuals( );
    void DeleteDead( );
    
    Nutrient& mNutrient;
    Autotrophs& mAutotrophs;

    HeterotrophProcessor mHeterotrophProcessor;
    HeterotrophData mHeterotrophData;
    
    RandomSimple mRandom;
    Timer mTimer;
    
    const Types::DoubleMatrix mInterSizeClassPreferenceMatrix;
    const Types::DoubleMatrix mInterSizeClassVolumeMatrix;
    
    const Types::DoubleVector mSizeClassMidPoints;
    const Types::UnsignedVector mMaximumSizeClassPopulations;

    const double mSmallestIndividualVolume;
    const double mInitialHeterotrophicVolume;
    const double mSizeClassSubsetFraction;

    const unsigned mNumberOfSizeClasses;
    const unsigned mPreferredPreyVolumeRatio;
    const unsigned mAutotrophSizeClassIndex;

    Types::UnsignedVector mFedCount;
    
    Types::HeterotrophVector mChildren;
    Types::HeterotrophIndexVector mIndividualsToMove;

    Types::HeterotrophMatrix mIndividuals;
    Types::HeterotrophMatrix mIndividualsDead;
    
    TestStruct* mTestStructure;
};

#endif

