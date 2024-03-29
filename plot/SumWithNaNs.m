%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Evolutionary Adaptive Trophic Structure Model (EATSM)                       %
%                                                                             %
% (C) Copyright 2024, Phil Underwood. All rights reserved.                    %
%                                                                             %
% This software is licensed under the terms of the 3-Clause BSD License       %
% which can be obtained from https://opensource.org/license/bsd-3-clause/.    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
function mean = SumWithNaNs( nanMatrix, dimension )

isNaNMatrix = isnan( nanMatrix );
noNaNMatrix = nanMatrix;
noNaNMatrix( isNaNMatrix == 1 ) = 0;

if nargin == 1,
    mean = sum( noNaNMatrix );
else
    mean = sum( noNaNMatrix, dimension );
end