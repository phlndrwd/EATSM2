%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Evolutionary Adaptive Trophic Structure Model (EATSM)                       %
%                                                                             %
% (C) Copyright 2024, Phil Underwood. All rights reserved.                    %
%                                                                             %
% This software is licensed under the terms of the 3-Clause BSD License       %
% which can be obtained from https://opensource.org/license/bsd-3-clause/.    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
dataSetListing = dir( [ optionOutputDirectory optionCurrentDataSet ] );

for dataFileCount = 1:length( dataSetListing )
    dataFileName = dataSetListing( dataFileCount ).name;
    if dataSetListing( dataFileCount ).isdir == 1 && strcmp( dataFileName( 1 ), '.' ) ~= 1
       
        
        dataInputFile = [ optionOutputDirectory optionCurrentDataSet dataFileName ];
        
        disp( dataInputFile );
    end
end