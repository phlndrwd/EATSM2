tic
clear

%% User Defined Parameters
optionCurrentDataSet            = '1e6';
optionOutputDirectory           = '/home/philju/Development/In Progress/EATSM/EATSM2/build/Release/output/0.0.1/';

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