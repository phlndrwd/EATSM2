function[ paddedMatrix ] = PadMatrix( matrix )

currentDimensions = size( matrix );
paddedMatrix = NaN( currentDimensions( 1 ) + 1, currentDimensions( 2 ) + 1);
paddedMatrix( 1:currentDimensions( 1 ), 1:currentDimensions( 2 ) ) = matrix;