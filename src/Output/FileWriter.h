#ifndef FILEWRITER
#define	FILEWRITER

#include "Environment.h"

class FileWriter {
public:
    FileWriter( );
    ~FileWriter( );
    
    void WriteOutputData( Environment& );

private:
    void InitialiseOutputDirectory( );
    bool WriteInputFiles( );
    
    bool WriteVectorDatums( );
    bool WriteMatrixDatums( );
    bool WriteStateFile( Environment& );

    std::string mOutputPath;
    std::string mDataSetDirectoryName;
};

#endif

