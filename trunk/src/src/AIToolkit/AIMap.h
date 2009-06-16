#ifndef _BITMAP_H
#define _BITMAP_H

#include <iostream>
#include <cstdio>
#include <string>
#include <math.h>
#include <map>
#include <vector>
using namespace std;


/////////////////////////////////
/////////////////////////////////
#pragma pack(push, bitmap_data, 1)

typedef struct tagBitmapFileHeader {
	unsigned short	bfType;
	unsigned int	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned int	bfOffBits;
} BitmapFileHeader;

typedef struct tagBitmapInfoHeader {
	unsigned int	biSize;
	int				biWidth;
	int				biHeight;
	unsigned short	biPlanes;
	unsigned short	biBitCount;
	unsigned int	biCompression;
	unsigned int	biSizeImage;
	int				biXPelsPerMeter;
	int				biYPelsPerMeter;
	unsigned int	biClrUsed;
	unsigned int	biClrImportant;
} BitmapInfoHeader;

#pragma pack(pop, bitmap_data)
/////////////////////////////////
/////////////////////////////////

struct colorAIMap
{
	int R;
	int G;
	int B;
};


class Bitmap
{
private:
	static const int MAX_TAB = 36;

public:
    Bitmap();
    ~Bitmap();

    bool loadBMP(char *);
	void convertImgToMatrix( int tabImg[MAX_TAB][MAX_TAB] );

private:
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;

    unsigned int	dataSize;
	unsigned short	bpp;
    bool			loaded;
	int				height;
    int				width;  
	int				byteWidth;
    int				padWidth;

	int tabColor[256][256];
	vector<pair<int,int>>	listSpawnPoint;
	pair<int,int>			lastSpawnPoint;

	colorAIMap color;
	int row;
	int col;

    bool convert24(unsigned char*);
};

#endif //_BITMAP_H

