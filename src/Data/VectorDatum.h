#ifndef VECTORDATUM
#define	VECTORDATUM

#include "Constants.h"
#include "Types.h"

class VectorDatum {
public:
    VectorDatum( const std::string& );
    ~VectorDatum( );
    
    float GetDataAtIndex( const unsigned& ) const;
    
    void SetData( const Types::FloatVector );
    void AddData( const float& );
    
    unsigned GetSize( ) const;
    const std::string& GetName( ) const;

private:
    Types::FloatVector mData;
    std::string mName;
};

#endif

