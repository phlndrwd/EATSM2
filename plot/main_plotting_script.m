%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Evolutionary Adaptive Trophic Structure Model (EATSM)                       %
%                                                                             %
% (C) Copyright 2024, Phil Underwood. All rights reserved.                    %
%                                                                             %
% This software is licensed under the terms of the 3-Clause BSD License       %
% which can be obtained from https://opensource.org/license/bsd-3-clause/.    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tic
clear

%% User Defined Parameters
optionCurrentDataSet            = '2024-04-16_02-10-31';
%optionOutputDirectory           = '/home/philju/Work/Writing/EATSM/Experiments/SpeedMutationTest/0.5.3/02/';
optionOutputDirectory           = '/home/philju/Development/Repositories/EATSM2/build/Debug/output/0.0.1/';

optionOutputVariablesFile      = 'Variables';
optionFileExtension             = '.csv';

optionPrintPlotsToFile          = 1; % yes = 1, no = anything else
optionOutputFileFormat          = 'png'; % EPS or PNG
optionPlotImageWidth            = 12; % cm
optionPlotImageHeight           = 10; % cm

optionMissingValue              = -9999;
optionTruncateTimeAt            = 0; % Index not value
optionResampleTimeTo            = 0; % Value not index

%% Input Formatting
if strcmp( optionOutputDirectory( end ), '/' ) == 0
    optionOutputDirectory = [ optionOutputDirectory '/' ];
end

if strcmp( optionCurrentDataSet( end ), '/' ) == 0
    optionCurrentDataSet = [ optionCurrentDataSet '/' ];
end

%% Plot Script Execution
if exist( [ optionOutputDirectory optionCurrentDataSet ], 'dir' ) == 7
    plot_data
else
    disp( [ 'ERROR> Input directory "' optionOutputDirectory optionCurrentDataSet '" does not exist.' ] );
end
toc