function[ fileName ] = ReturnFileNameFromFullPath( filePath )

count = 0;

for index = length( filePath ):-1:1
    if strcmp( filePath( index ), '/' ) == 1
        count = count - 1;
        break;
    end
    count = count + 1;
end

fileName = filePath( end-count:end );
