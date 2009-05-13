#ifndef		_Color_H
#define		_Color_H

#include	<d3dx9math.h>

//******************************************************************

// Couleur en 4 flottants : rgba ;
// les valeurs sont comprisent entre 0.f et 1.f
typedef D3DXCOLOR Color4f;

//******************************************************************

// Convertie une Color4f en D3DCOLOR
#define	D3DCOLOR_COLOR4F( color ) \
	D3DCOLOR_COLORVALUE((color).r,(color).g,(color).b,(color).a)

//******************************************************************
#endif		// _Color_H
