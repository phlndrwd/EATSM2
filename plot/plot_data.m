%% Create Meta Data
labelVolumeAxis = 'Equivalent Spherical Volume';
labelTimeAxis = 'Time Steps';

%% Reserved Variable Names
searchTermApproxVolume = 'approxvolume';
searchTermCouplings = 'couplings';
searchTermMidPoint = 'midpoint';
searchTermPopulation = 'population';
searchTermTimeAxis = 'axistime';
searchTermTrophic = 'trophic';
searchTermValues = 'values';
searchTermVolume = 'volume';
searchTermTiming = 'timing';

%% Trophic Level Names
trophicLevelNames{ 1 } = 'Secondary';
trophicLevelNames{ 2 } = 'Tertiary';
trophicLevelNames{ 3 } = 'Quaternary';
trophicLevelNames{ 4 } = 'Quinary';
trophicLevelNames{ 5 } = 'Senary';
trophicLevelNames{ 6 } = 'Septenary';
trophicLevelNames{ 7 } = 'Octonary';
trophicLevelNames{ 8 } = 'Nonary';
trophicLevelNames{ 9 } = 'Denary';

%% Load Meta Data
OutputVariables = ReadTable( [ optionOutputDirectory optionCurrentDataSet optionOutputVariablesFile optionFileExtension ], ',' );


%% Load Data
numberOfDatums = length( OutputVariables( :, 1 ) );

volumeMatrixIndex = 3;
timingMatrixIndex = 4;

for datumIndex = 1:numberOfDatums
    dataSetName = OutputVariables{ datumIndex, 1 };
    dataInputFile = [ optionOutputDirectory optionCurrentDataSet dataSetName optionFileExtension ];
    
    if exist( dataInputFile, 'file' ) == 2
        %% Load data
        load( dataInputFile, '-ascii' );
        dataSet = eval( dataSetName );
        %% Truncation and resampling
        if ~contains(  lower( dataSetName ), searchTermValues  ) % Data is not size class vectors
            if optionTruncateTimeAt > 0 && optionTruncateTimeAt < length( dataSet )
                if numel(  dataSet  ) == length( dataSet ) % Data is 1-dimensional
                    dataSet = dataSet( 1:optionTruncateTimeAt );
                else                                            % Data is n-dimensional
                    dataSet = dataSet( :, 1:optionTruncateTimeAt );
                end
            end
            if optionResampleTimeTo > 0
                resamplingMethod = OutputVariables{ datumIndex, 4 };
                if strcmpi( resamplingMethod, 'cumulative' ) == 1
                    dataSet = ResampleCumulativeMatrix( dataSet, optionResampleTimeTo );
                elseif strcmpi( resamplingMethod, 'standard' ) == 1
                    dataSet = ResampleMatrix( dataSet, optionResampleTimeTo );
                end
            end
            if contains(  lower( dataSetName ), searchTermTimeAxis  ) % Is time axis
                AxisTimeSteps = dataSet;
                AxisTimeStepsExtended = ExtendVector( AxisTimeSteps );
                maximumTime = max( AxisTimeSteps );
                volumeMatrix = zeros( length( AxisTimeSteps ), volumeMatrixIndex );
                timingMatrix = zeros( length( AxisTimeSteps ), timingMatrixIndex );
            else                                                      % Is data for plotting
                %% Plotting
                plotType = OutputVariables{ datumIndex, 2 };
                dataLabel = OutputVariables{ datumIndex, 3 };
                
                handle = figure;
                
                if strcmp( plotType, 'vector' ) == 1
                    %% Line plots
                    plot( AxisTimeSteps, dataSet );
                    ylabel( dataLabel );
                    
                    if contains(  lower( dataSetName ), searchTermVolume  ) && ~contains(  lower( dataSetName ), searchTermApproxVolume  )
                        % Data collection for volume area plot
                        volumeMatrix( :, volumeMatrixIndex ) = dataSet;
                        volumeMatrixIndex = volumeMatrixIndex - 1;
                    elseif contains( lower( dataSetName ), searchTermTiming ) == 1
                        % Data collection for timing area plot
                        timingMatrix( :, timingMatrixIndex ) = dataSet;
                        timingMatrixIndex = timingMatrixIndex - 1;
                    end
                    
                elseif strcmp( plotType, 'matrix' ) == 1
                    dataSet( dataSet == optionMissingValue ) = NaN;
                    
                    if ~contains(  lower( dataSetName ), searchTermCouplings  )
                        dataSet( dataSet == 0 ) = NaN;
                    end
                    
                    if contains(  lower( dataLabel ), 'log_{10}'  )
                        dataSet = log10( dataSet );
                    elseif contains(  lower( dataLabel ), 'log_{2}'  )
                        dataSet = log2( dataSet );
                    elseif contains(  lower( dataLabel ), 'log'  )
                        dataSet = log( dataSet );
                    end
                    
                    if strfind( lower( dataSetName ), searchTermTrophic ) == 1
                        %% Trophic plots
                        dataSet( all( isnan( dataSet ), 2 ), : ) = []; % Remove rows where all data are missing
                        sizeDataSet = size( dataSet );
                        numberOfTrophicLevels = sizeDataSet( 1 ) - 1;
                        if numberOfTrophicLevels > 0 % No trophic levels when no feeding events have occurred
                            dataSet = dataSet( 2:sizeDataSet( 1 ), : );
                            plot( AxisTimeSteps, dataSet' ), shading flat;
                            ylabel( dataLabel );
                            legend( trophicLevelNames( 1:numberOfTrophicLevels ) );
                        end
                    else
                        %% Size class plots
                        dataSet = PadMatrix( dataSet ); % Extend matrix for pcolor plot
                        
                        pcolor( AxisTimeStepsExtended, AxisSizeClassBoundaryValues, dataSet ), shading flat;
                        
                        set( gca, 'YScale', 'log' );
                        c = colorbar;
                        
                        minVal = min( min( dataSet ) );
                        maxVal = max( max( dataSet ) );
                        
                        if contains(  lower( dataSetName ), searchTermCouplings  )
                            caxis( [ 0, traitResolution ] )
                        elseif minVal < maxVal
                            caxis( [ minVal, maxVal ] )
                        end
                        ylabel( c, dataLabel );
                        ylabel( labelVolumeAxis );
                    end
                end
                xlim( [ 0 maximumTime ] );
                grid on; set( gca,'layer','top' );
                title( dataSetName );
                xlabel( labelTimeAxis );
                if optionPrintPlotsToFile == 1
                    printPlotToFile( handle, [ optionPlotImageWidth optionPlotImageHeight ], [ optionOutputDirectory optionCurrentDataSet dataSetName ], optionOutputFileFormat );
                    close( handle );
                end
            end
            eval( [ dataSetName ' = dataSet;' ] ); % Necessary to overwrite original data.
        elseif contains(  lower( dataSetName ), searchTermMidPoint  ) % Data is mid-point values vector
            traitResolution = length( AxisSizeClassMidPointValues );
        end
    else
        disp( [ 'File "' dataInputFile '" does not exist.' ] );
    end
end

%% Additional plots

% Stock Volumes
if volumeMatrixIndex == 0
    dataSetName = 'StockVolumes';
    handle = figure;
    area( AxisTimeSteps, volumeMatrix );
    legend( 'Nutrient', 'Autotrophs', 'Heterotrophs');
    xlim( [ 0 maximumTime ] );
    ylim( [ 0 max( sum( volumeMatrix, 2 ) ) ] );
    grid on; set( gca,'layer','top' );
    title( dataSetName );
    xlabel( labelTimeAxis );
    ylabel( 'Volume' );
    if optionPrintPlotsToFile == 1
        printPlotToFile( handle, [ optionPlotImageWidth optionPlotImageHeight ], [ optionOutputDirectory optionCurrentDataSet dataSetName ], optionOutputFileFormat );
        close( handle );
    end
end

if timingMatrixIndex == 0
    dataSetName = 'Timing';
    handle = figure;
    area( AxisTimeSteps, timingMatrix );
    %legend( 'Feeding', 'Metabolisation', 'Starvation', 'Reproduction', 'Location', 'NorthWest' );
    legend( 'Starvation', 'Reproduction', 'Metabolisation', 'Feeding', 'Location', 'NorthWest' );
    xlim( [ 0 maximumTime ] );
    ylim( [ 0 max( sum( timingMatrix, 2 ) ) ] );
    grid on; set( gca,'layer','top' );
    title( dataSetName );
    xlabel( labelTimeAxis );
    ylabel( 'Time (s)' );
    if optionPrintPlotsToFile == 1
        printPlotToFile( handle, [ optionPlotImageWidth optionPlotImageHeight ], [ optionOutputDirectory optionCurrentDataSet dataSetName ], optionOutputFileFormat );
        close( handle );
    end                      
end