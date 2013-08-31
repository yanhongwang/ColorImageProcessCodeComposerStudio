#include <math.h>
#include "AutoWhiteBalance.h"

void ColorTemperatureCurve
(
	
	unsigned char * ImageDataBlock,
	int imageHeight,
	int imageWidth,
	double * RCoefficient,
	double * BCoefficient,
	int ColumnWidth
	
)
{
	
	int i;
	int j;
	
	int BIndex;
	int GIndex;
	int RIndex;
	
	double x;
	double y;
	
	// CTC coefficient
	double slope;
	double b;
	
	double GRRatio = 0;
	double GBRatio = 0;
	
	double RTotal = 0;
	double GTotal = 0;
	double BTotal = 0;
	
	for( i = 0; i < imageHeight; i ++ )
		for( j = 0; j < ColumnWidth; j += 3 )
		{
			
			BIndex = i * ColumnWidth + ( j + 0 );
			GIndex = i * ColumnWidth + ( j + 1 );
			RIndex = i * ColumnWidth + ( j + 2 );
			
			BTotal += ImageDataBlock[ BIndex ];
			GTotal += ImageDataBlock[ GIndex ];
			RTotal += ImageDataBlock[ RIndex ];
			
		}
	
	GBRatio = GTotal / BTotal;
	GRRatio = GTotal / RTotal;
	
	GBRatio = log10( GBRatio );
	GRRatio = log10( GRRatio );
	
	x = GRRatio;
	y = GBRatio;
	
	// New Lctc : y = -1.0163x + 0.4258
	slope = 1 / 1.0163;
	b = y - slope * x;
	
	GRRatio = ( -b + 0.4258 ) / ( slope + 1.0163 );
	GBRatio = -1.0163 * GRRatio + 0.4258;
	
	*RCoefficient = pow( 10, GRRatio );
	*BCoefficient = pow( 10, GBRatio );
	
}
