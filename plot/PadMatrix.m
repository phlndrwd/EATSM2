%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Evolutionary Adaptive Trophic Structure Model (EATSM)                       %
%                                                                             %
% (C) Copyright 2024, Phil Underwood. All rights reserved.                    %
%                                                                             %
% This software is licensed under the terms of the 3-Clause BSD License       %
% which can be obtained from https://opensource.org/license/bsd-3-clause/.    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
function[ paddedMatrix ] = PadMatrix( matrix )

currentDimensions = size( matrix );
paddedMatrix = NaN( currentDimensions( 1 ) + 1, currentDimensions( 2 ) + 1);
paddedMatrix( 1:currentDimensions( 1 ), 1:currentDimensions( 2 ) ) = matrix;