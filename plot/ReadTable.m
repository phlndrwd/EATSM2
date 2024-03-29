%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Evolutionary Adaptive Trophic Structure Model (EATSM)                       %
%                                                                             %
% (C) Copyright 2024, Phil Underwood. All rights reserved.                    %
%                                                                             %
% This software is licensed under the terms of the 3-Clause BSD License       %
% which can be obtained from https://opensource.org/license/bsd-3-clause/.    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function[ data ] = ReadTable( filePath, delimiterCharacter )

fid = fopen( filePath );

rowCount = 0;
while ~feof( fid )
    line = fgetl( fid );
    if rowCount == 0
        lineCells = strsplit( line, delimiterCharacter );
        columnCount = length( lineCells );
    end
    rowCount = rowCount + 1;
end

fclose( fid );
fid = fopen( filePath );
data = cell( rowCount - 1, columnCount );

rowCount = 0;
while ~feof( fid )
    line = fgetl( fid );
    if rowCount > 0
        lineCells = strsplit( line, delimiterCharacter );
        data( rowCount, : ) = lineCells;
    end
    rowCount = rowCount + 1;
end

fclose( fid );