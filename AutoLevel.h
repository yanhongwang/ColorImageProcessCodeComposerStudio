#define	AutoLevelLowThreshold	0.001
#define	AutoLevelHighThreshold	0.001
#define	LowLevelLimitation		20
#define	HighLevelLimitation		950

void RGBThreshold( unsigned char  * ImageDataBlock, int imageHeight, int imageWidth, int * LowThreshold, int * HighThreshold, int ColumnWidth );
