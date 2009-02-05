#ifndef		_Color_H
#define		_Color_H

#include	<d3d9.h>

//******************************************************************

// Couleur en 4 flottants ;
// les valeurs sont comprisent entre 0.f et 1.f

struct Color4f
{
	float r, g, b, a;

	// Constructeurs
	Color4f(){}
	Color4f( float cr, float cg, float cb, float ca );

};

//******************************************************************

// Convertie une Color4f en D3DCOLOR
#define	D3DCOLOR_COLOR4F( color ) \
	D3DCOLOR_COLORVALUE((color).r,(color).g,(color).b,(color).a)

//******************************************************************
#endif		// _Color_H
