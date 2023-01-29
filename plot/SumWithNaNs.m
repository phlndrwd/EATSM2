function mean = SumWithNaNs( nanMatrix, dimension )

isNaNMatrix = isnan( nanMatrix );
noNaNMatrix = nanMatrix;
noNaNMatrix( isNaNMatrix == 1 ) = 0;

if nargin == 1,
    mean = sum( noNaNMatrix );
else
    mean = sum( noNaNMatrix, dimension );
end