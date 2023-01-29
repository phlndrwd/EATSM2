#ifndef TYPES
#define TYPES

#include <fstream>     // For std::i/ofstream
#include <map>         // For map
#include <sys/stat.h>  // For mkdir
#include <sys/time.h>  // For struct timeval
#include <vector>      // For std::vector

class DataRecorder;
class Heterotroph;
class InitialState;
class MatrixDatum;
class Parameters;
class VectorDatum;

namespace Types {
// Pointers
typedef DataRecorder* DataRecorderPointer;
typedef Heterotroph* HeterotrophPointer;
typedef InitialState* InitialStatePointer;
typedef MatrixDatum* MatrixDatumPointer;
typedef Parameters* ParametersPointer;
typedef VectorDatum* VectorDatumPointer;

// Containers of class pointers
typedef std::map<std::string, MatrixDatumPointer> MatrixDatumMap;
typedef std::map<std::string, VectorDatumPointer> VectorDatumMap;

typedef std::vector<HeterotrophPointer> HeterotrophVector;
typedef std::vector<std::pair<HeterotrophPointer, unsigned> > HeterotrophIndexVector;

// Containers of containers of class pointers
typedef std::vector<HeterotrophVector> HeterotrophMatrix;

// Containers of primitives
typedef std::vector<bool> BoolVector;
typedef std::vector<double> DoubleVector;
typedef std::vector<float> FloatVector;
typedef std::vector<int> IntegerVector;
typedef std::vector<std::string> StringVector;
typedef std::vector<unsigned> UnsignedVector;

// Containers of containers of primitives
typedef std::vector<IntegerVector> IntegerMatrix;
typedef std::vector<FloatVector> FloatMatrix;
typedef std::vector<DoubleVector> DoubleMatrix;
typedef std::vector<StringVector> StringMatrix;
typedef std::vector<UnsignedVector> UnsignedMatrix;
}  // namespace Types

#endif
