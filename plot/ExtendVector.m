function[ vector ] = ExtendVector( vector )

length_vector = length( vector );
vector( length_vector + 1 ) = vector( length_vector ) + ( vector( length_vector ) - vector( length_vector - 1 ) );
