%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Evolutionary Adaptive Trophic Structure Model (EATSM)                       %
%                                                                             %
% (C) Copyright 2024, Phil Underwood. All rights reserved.                    %
%                                                                             %
% This software is licensed under the terms of the 3-Clause BSD License       %
% which can be obtained from https://opensource.org/license/bsd-3-clause/.    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
function[ vector ] = ExtendVector( vector )

length_vector = length( vector );
vector( length_vector + 1 ) = vector( length_vector ) + ( vector( length_vector ) - vector( length_vector - 1 ) );
