dataSetListing = dir( [ optionOutputDirectory optionCurrentDataSet ] );

for dataFileCount = 1:length( dataSetListing )
    dataFileName = dataSetListing( dataFileCount ).name;
    if dataSetListing( dataFileCount ).isdir == 1 && strcmp( dataFileName( 1 ), '.' ) ~= 1
       
        
        dataInputFile = [ optionOutputDirectory optionCurrentDataSet dataFileName ];
        
        disp( dataInputFile );
    end
end