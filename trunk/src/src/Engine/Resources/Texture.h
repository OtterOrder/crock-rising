#ifndef		_Texture_H
#define		_Texture_H

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	"Resource.h"

//===========================================================================//
// Defines & enums                                                           //
//===========================================================================//

enum TextureType
{
	TEX_MESH = 1,	// (on commence à 1 pour ne pas confondre avec NULL = 0)
	TEX_NORMALMAP,
	TEX_BUMPMAP,
	TEX_DISPLACEMENTMAP,
	
	TEX_SPRITE
};

//===========================================================================//
// Classe texture                                                            //
//===========================================================================//
class Texture: public Resource
{
public:
	
	Texture();
	~Texture();
	
	ResourceResult Load( std::string resource, ResourceParam param ); // Charge une texture
	
	LPDIRECT3DTEXTURE9 m_pTex; // Pointeur vers la texture

protected:

	ResourceResult DxLoad( std::string path );

};

#endif		//_Texture_H
