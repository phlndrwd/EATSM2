function[ resampledCumulativeMatrix ] = ResampleCumulativeMatrix( matrix, newNDimension )

vectorMatrixDimensions = size( matrix );

if newNDimension < vectorMatrixDimensions( 2 )
    timeIncrement = vectorMatrixDimensions( 2 ) / newNDimension;
    
    newTimeIndex = 0;
    lastSampledIndex = 1;
    resampledCumulativeMatrix = zeros( vectorMatrixDimensions( 1 ), newNDimension );
    
    for timeIndex = 1:vectorMatrixDimensions( 2 )
        if timeIndex > ( newTimeIndex * timeIncrement )
            newTimeIndex = newTimeIndex + 1;
            resampledCumulativeMatrix( :, newTimeIndex ) = SumWithNaNs( matrix( :, lastSampledIndex:timeIndex ), 2 );
            lastSampledIndex = timeIndex;
        end
    end
else
    resampledCumulativeMatrix = matrix;
end