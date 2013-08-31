#include <stdio.h>
#include <math.h>
#include <time.h>

#include "bmp.h"
#include "AutoWhiteBalance.h"
#include "RGBCIELAB.h"

// Auto Level
#define HighThreshold	250
#define	LowThreshold	5
#define	AutoLevelRange	( float )( HighThreshold - LowThreshold )

// Color Saturation Enhancement
#define	aFactor	1.1
#define	bFactor	1.1

// Gamma Correction
#define	LightGamma	0.45

#define SDRAM_BASE 0x80000000

void main()
{
	
	int i;
	int j;
	
	int RIndex;
	int GIndex;
	int BIndex;
	
	int imageWidth;
	int imageHeight;
	int ColumnWidth;
	
	struct BitMapFileHeader header1;
	struct BitMapInfoHeader header2;
	
	unsigned char *ImageDataBlock;
	
	unsigned char Rtemp;
	unsigned char Gtemp;
	unsigned char Btemp;
	
	// AutoWhiteBalance
	double RCoefficient;
	double BCoefficient;
	float tempValue;
	
	// Color Correction
	float tempRValue;
	float tempGValue;
	float tempBValue;
	
	// sRGB and CIELAB
	double L;
	double a;
	double b;
	
	FILE *fptr;
	
	// algorithm efficiency measurement
	time_t	start;
	time_t	now;
	
	ImageDataBlock = ( unsigned char * )SDRAM_BASE; 
	
	fptr = fopen( "320_240.BMP", "rb" );
	
	fread( &header1, 14, 1, fptr );
	
	fread( &header2, 40, 1, fptr );
	
	imageWidth = header2.biWidth;
	imageHeight = header2.biHeight;
	ColumnWidth	= imageWidth * 3;
	
	fread( ImageDataBlock, imageHeight * ColumnWidth, sizeof( unsigned char ), fptr ); 
	
	fclose( fptr );
	
	// algorithm begin
	start = time( NULL );
	printf( "algorithm start\n" );
	
	// Auto Level
	for( i = 0; i < imageHeight; i ++ )
		for( j = 0; j < ColumnWidth; j += 3 )
		{
			
			BIndex = i * ColumnWidth + ( j + 0 );
			GIndex = i * ColumnWidth + ( j + 1 );
			RIndex = i * ColumnWidth + ( j + 2 );
			
			// B channel
			if( ( ImageDataBlock[ BIndex ] > LowThreshold ) && ( ImageDataBlock[ BIndex ] < HighThreshold ) )
				ImageDataBlock[ BIndex ] = 256.0 * ( ImageDataBlock[ BIndex ] - LowThreshold ) / AutoLevelRange;
			else if( ImageDataBlock[ BIndex ] <= LowThreshold )
				ImageDataBlock[ BIndex ] = 0;
			else	// ImageDataBlock[ BIndex ] >= HighThreshold
				ImageDataBlock[ BIndex ] = 255;
			
			// G channel
			if( ( ImageDataBlock[ GIndex ] > LowThreshold ) && ( ImageDataBlock[ GIndex ] < HighThreshold ) )
				ImageDataBlock[ GIndex ] = 256.0 * ( ImageDataBlock[ GIndex ] - LowThreshold ) / AutoLevelRange;
			else if( ImageDataBlock[ GIndex ] <= LowThreshold )
				ImageDataBlock[ GIndex ] = 0;
			else	// ImageDataBlock[ GIndex ] >= HighThreshold
				ImageDataBlock[ GIndex ] = 255;
			
			// R channel
			if( ( ImageDataBlock[ RIndex ] > LowThreshold ) && ( ImageDataBlock[ RIndex ] < HighThreshold ) )
				ImageDataBlock[ RIndex ] = 256.0 * ( ImageDataBlock[ RIndex ] - LowThreshold ) / AutoLevelRange;
			else if( ImageDataBlock[ RIndex ] <= LowThreshold )
				ImageDataBlock[ RIndex ] = 0;
			else	// ImageDataBlock[ RIndex ] >= HighThreshold
				ImageDataBlock[ RIndex ] = 255;
			
		}
	
	// Auto White Balance
	ColorTemperatureCurve
	(
		
		ImageDataBlock,
		imageHeight,
		imageWidth,
		&RCoefficient,
		&BCoefficient,
		ColumnWidth
		
	);
	
	for( i = 0; i < imageHeight; i ++ )
		for( j = 0; j < ColumnWidth; j += 3 )
		{	
			
			BIndex = i * ColumnWidth + ( j + 0 );
			GIndex = i * ColumnWidth + ( j + 1 );
			RIndex = i * ColumnWidth + ( j + 2 );
			
			// Auto White Balance
			tempValue = ImageDataBlock[ BIndex ] * BCoefficient / 1.15;
			Btemp = ( unsigned char )( tempValue < 0 ? 0 : tempValue >  255 ? 255 : tempValue );
			Gtemp = ImageDataBlock[ GIndex ];
			tempValue = ImageDataBlock[ RIndex ] * RCoefficient * 1.15;
			Rtemp = ( unsigned char )( tempValue < 0 ? 0 : tempValue >  255 ? 255 : tempValue );
			
			// Color Correction
			tempRValue = Rtemp * (  860.0 / 512 ) + Gtemp * ( -253.0 / 512 ) + Btemp * (  -95.0 / 512 );	// R element
			tempGValue = Rtemp * ( -109.0 / 512 ) + Gtemp * (  928.0 / 512 ) + Btemp * ( -307.0 / 512 );	// G element
			tempBValue = Rtemp * (   20.0 / 512 ) + Gtemp * ( -290.0 / 512 ) + Btemp * (  782.0 / 512 );	// B element
			Rtemp = ( unsigned char )( tempRValue < 0 ? 0 : tempRValue > 255 ? 255 : tempRValue );
			Gtemp = ( unsigned char )( tempGValue < 0 ? 0 : tempGValue > 255 ? 255 : tempGValue );
			Btemp = ( unsigned char )( tempBValue < 0 ? 0 : tempBValue > 255 ? 255 : tempBValue );
			
			// sRGB to CIELAB
			RGB2Lab
			(
				
				Btemp,
				Gtemp,
				Rtemp,
				&L,
				&a,
				&b
				
			);
			
			// Color Saturation Enhancement
			a *= aFactor;
			b *= bFactor;
			
			// CIELAB to sRGB
			Lab2RGB
			(
				
				L,
				a,
				b,
				&Btemp,
				&Gtemp,
				&Rtemp
				
			);
			
			// Gamma Correction
			ImageDataBlock[ BIndex ] = pow( Btemp / 256.0, LightGamma ) * 256.0;
			ImageDataBlock[ GIndex ] = pow( Gtemp / 256.0, LightGamma ) * 256.0;
			ImageDataBlock[ RIndex ] = pow( Rtemp / 256.0, LightGamma ) * 256.0;
			
		}
	
	// algorithm end
	now = time( NULL );
	printf( "Elapsed time = %d seconds\n", now - start );
	
	fptr = fopen( "result.bmp", "wb" );
	
	fwrite( &header1, 14, 1, fptr );
	
	fwrite( &header2, 40, 1, fptr );
	
	fwrite( ImageDataBlock, imageHeight * ColumnWidth, sizeof( unsigned char ), fptr );
	
	fclose( fptr );
	
	printf( "\ndone\n" );
	
}
