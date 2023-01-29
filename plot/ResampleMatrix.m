function[ resampledMatrix ] = ResampleMatrix( matrix, newNDimension )

vectorMatrixDimensions = size( matrix );

if newNDimension < vectorMatrixDimensions( 2 )
    timeIncrement = vectorMatrixDimensions( 2 ) / newNDimension;
    
    newTimeIndex = 0;
    resampledMatrix = zeros( vectorMatrixDimensions( 1 ), newNDimension );
    
    for timeIndex = 1:vectorMatrixDimensions( 2 )
        if timeIndex > ( newTimeIndex * timeIncrement )
            newTimeIndex = newTimeIndex + 1;
            resampledMatrix( :, newTimeIndex ) = matrix( :, timeIndex );
        end
    end
else
    resampledMatrix = matrix;
end
