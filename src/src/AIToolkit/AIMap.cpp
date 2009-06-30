#include "AIMap.h"

Bitmap::Bitmap()	:	loaded(false), row(0), col(0)
{
}

Bitmap::~Bitmap()
{
}

bool Bitmap::loadBMP(char *file) 
{
    FILE *in;
    unsigned char *tempData;

    // Ouvre le fichier en binaire
    in = fopen(file,"rb");
    if(in == NULL) 
	{
        return false;
    }

    // Lit les infos du fichier
    fread(&bmfh,sizeof(BitmapFileHeader),1,in);

    // 19778 vérifie le format BMP
    if(bmfh.bfType!=19778) 
	{
        fclose(in);
        return false;
    }

    // Lit les infos de l'entete du bitmap
    fread( &bmih, sizeof(BitmapInfoHeader), 1, in );
    width	= bmih.biWidth;
    height	= bmih.biHeight;
    bpp		= bmih.biBitCount;

    // Calcul la taille de l'image avec le padding
    dataSize=(width*height*(unsigned int)(bmih.biBitCount/8.0));

    // Si la bitmap a moins de 8bits par pixel format incorrect 
    if(bpp<8) 
	{
        fclose(in);
        return false;
    }

    tempData = new unsigned char[dataSize];		// Creer le "tableau" temporaire qui contiendra la couleur des pixels
    fread(tempData,sizeof(char),dataSize,in);	// Lit les pixels de l'image  

    // Calcul la largeur de l'image final en bit
    byteWidth = padWidth = (int)((float)width*(float)bpp/8.0);

    // Ajuste la largueur par rapport au padding si neccessaire
    while(padWidth%4!=0)	padWidth++;

    loaded = convert24(tempData);

	fclose(in);
    delete[] tempData;

    return loaded;
}


bool Bitmap::convert24(unsigned char* tempData) 
{
    if( height > 0 ) 
	{
        int offset = padWidth - byteWidth;
       
        for(unsigned int i=0; i<dataSize; i+=3) 
		{
			// Réajuste le padding des bitmap
            if((i+1)%padWidth==0)	i+=offset;

            // Le format BMP est sous forme BGR => on transforme en meme temps en RGB
			color.R = (int)*(tempData+i+2);
			color.G = (int)*(tempData+i+1);
			color.B = (int)*(tempData+i+0);

			// Test si l'image est bien en noir/blanc/rouge
			if (	(color.R == 0 || color.R == 255 || color.R == 250)
				&&	(color.G == 0 || color.G == 255)
				&&	(color.B == 0 || color.B == 255) )
			{
				if ( color.R == 250 )
				{
					lastSpawnPoint.first = 256-row;
					lastSpawnPoint.second = col;
					listSpawnPoint.push_back(lastSpawnPoint);
				}

				tabColor[row][col] = color.R;
				col++;
				
				if (col == 256)
				{
					col = 0;
					row++;
				}
			}
			else
				return false;
        }
    }
    else	
		return false;

    return true;
}

void Bitmap::convertImgToMatrix( int tabImg[MAX_TAB][MAX_TAB] )
{
	int tmpSize = 256/MAX_TAB;

	// Transforme les valeurs 0 et 255 en 0 et 1 et inverse l'image pour qu'elle soit a l'endroit
	for (int i=0; i<MAX_TAB; i++)
	for (int j=0; j<MAX_TAB; j++)
	{
		tabImg[MAX_TAB-i-1][j] = ceil((float)tabColor[i*tmpSize][j*tmpSize]/255.f);		// Tableau inversé
	}
}