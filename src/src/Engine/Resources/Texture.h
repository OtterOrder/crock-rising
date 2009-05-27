#ifndef		_Texture_H
#define		_Texture_H

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	"Resource.h"

//===========================================================================//
// Defines & enums                                                           //
//===========================================================================//


//===========================================================================//
// Classe texture                                                            //
//===========================================================================//
class Texture: public Resource
{
public:
	enum Type
	{
		DIFFUSE = 1,	// (on commence à 1 pour ne pas confondre avec NULL = 0)
		NORMALMAP,
		BUMPMAP,
		DISPLACEMENTMAP,
		ALPHAMAP,
		
		SPRITE
	};
	
	LPDIRECT3DTEXTURE9	m_pTex;	// Pointeur sur la texture Dx
	
	Texture();
	~Texture();
	
	ResourceResult Load( std::string resource, ResourceParam param ); // Charge une texture

	int GetSrcWidth() const;	// Donne la largeur de l'image source
	int GetSrcHeight() const;	// Donne la hauteur de l'image source

protected:

	D3DXIMAGE_INFO	m_SrcInfo;	// Infos sur l'image source

	ResourceResult DxLoad( std::string path );

};

#endif		//_Texture_H
