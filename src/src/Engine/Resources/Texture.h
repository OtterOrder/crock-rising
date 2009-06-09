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
		DIFFUSE = 1,	// (on commence � 1 pour ne pas confondre avec NULL = 0)
		NORMALMAP,
		BUMPMAP,
		DISPLACEMENTMAP,
		ALPHAMAP,
		SPRITE
	};
	
	Texture();
	~Texture();
	
	ResourceResult Load( std::string resource, ResourceParam param ); // Charge une texture

	IDirect3DTexture9* const GetTexture() const;	// Donne la texture DirectX
	void SetTexture( IDirect3DTexture9 *pTexture );	// Change la texture DirectX

	int GetSrcWidth() const;	// Donne la largeur de l'image source
	int GetSrcHeight() const;	// Donne la hauteur de l'image source

protected:

	IDirect3DTexture9	*m_pTex;	// Pointeur sur la texture Dx
	D3DXIMAGE_INFO		m_SrcInfo;	// Infos sur l'image source

	ResourceResult DxLoad( std::string path );

};

#endif		//_Texture_H
