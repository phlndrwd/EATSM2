#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Nutrient.h"
#include "Autotrophs.h"
#include "Heterotrophs.h"

class Environment {
public:
    Environment( );
    ~Environment( );

    void Update( );
    bool RecordData( );

    Nutrient& GetNutrient( );
    Autotrophs& GetAutotrophs( );
    Heterotrophs& GetHeterotrophs( );

private:
    Nutrient mNutrient;
    Autotrophs mAutotrophs;
    Heterotrophs mHeterotrophs;
};

#endif

