#include <string.h>
#include "AutoLevel.h"
#include <stdio.h>

void RGBThreshold
(
	
	unsigned char * ImageDataBlock,
	int imageHeight,
	int imageWidth,
	int * LowThreshold,
	int * HighThreshold,
	int ColumnWidth
	
)
{
	
	int i;
	int j;
	
	int RIndex;
	int GIndex;
	int BIndex;
	
	int RHistogram[ 256 ];
	int GHistogram[ 256 ];
	int BHistogram[ 256 ];
	
	memset( RHistogram, 0, sizeof( RHistogram ) );
	memset( GHistogram, 0, sizeof( GHistogram ) );
	memset( BHistogram, 0, sizeof( BHistogram ) );
	/*
	for( i = 0; i < 200; i ++ )
	{
		
		//printf( "%d\n", RHistogram[ i ] );
		puts( "jjj\n" );
		RHistogram[ i ] = 0;
		GHistogram[ i ] = 0;
		BHistogram[ i ] = 0;
		
	}
	*/
	
	for( i = 0; i < imageHeight; i ++ )
		for( j = 0; j < ColumnWidth; j += 3 )
		{	
			
			BIndex = i * ColumnWidth + ( j + 0 );
			GIndex = i * ColumnWidth + ( j + 1 );
			RIndex = i * ColumnWidth + ( j + 2 );
			
			BHistogram[ ImageDataBlock[ BIndex ] ] ++;
			GHistogram[ ImageDataBlock[ GIndex ] ] ++;
			RHistogram[ ImageDataBlock[ RIndex ] ] ++;
			
		}
	
	/*
	// low level
	int Accumulation = 0;
	
	while( Accumulation <= imageHeight * imageWidth * AutoLevelLowThreshold )
		Accumulation += Histogram[ ( *LowThreshold ) ++ ];
	
	if(  * LowThreshold > LowLevelLimitation )
		* LowThreshold = LowLevelLimitation;
	
	// high level
	Accumulation = 0;
	
	while( Accumulation <= imageHeight * imageWidth * AutoLevelHighThreshold )
		Accumulation += Histogram[ ( *HighThreshold ) -- ];
	
	if(  * HighThreshold > HighLevelLimitation )
		* HighThreshold = HighLevelLimitation;
	*/
}
