%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Evolutionary Adaptive Trophic Structure Model (EATSM)                       %
%                                                                             %
% (C) Copyright 2024, Phil Underwood. All rights reserved.                    %
%                                                                             %
% This software is licensed under the terms of the 3-Clause BSD License       %
% which can be obtained from https://opensource.org/license/bsd-3-clause/.    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function printPlotToFile( plotHandle, imageDimensions, filePath, fileFormat, paintRenderer )

set( plotHandle,'PaperUnits','centimeters', 'PaperSize', imageDimensions, 'PaperPosition', [ 0 0 imageDimensions( 1 ) imageDimensions( 2 ) ] );

if length( ReturnFileNameExtension( ReturnFileNameFromFullPath( filePath ) ) ) > 0
    outputPath = filePath;
else
    outputPath = [ filePath '.' lower( fileFormat ) ];
end

if strcmpi( fileFormat, 'eps' ) == 1
    if exist( 'paintRenderer', 'var' ) ~= 1
        print( plotHandle, outputPath, '-depsc', '-loose' );
    else
        print( plotHandle, outputPath, '-depsc', '-loose', '-painters' );
    end
elseif strcmpi( fileFormat, 'png' ) == 1
    if exist( 'paintRenderer', 'var' ) ~= 1
        print( plotHandle, outputPath, '-dpng', '-r300' );
    else
        print( plotHandle, outputPath, '-dpng', '-r300', '-painters' );
        print( plotHandle, outputPath, '-dpng', '-r300' );
    end
end
