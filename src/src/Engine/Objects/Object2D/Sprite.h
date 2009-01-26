#ifndef		_Sprite_H
#define		_Sprite_H

//******************************************************************

#include	"Object2D.h"

//******************************************************************

class Sprite : public Object2D
{
public:

	//Sprite( crc32 spriteID );
	Sprite( const char *path );
	virtual ~Sprite();

	virtual void Draw() const;		// Affiche l'objet

	// Méthodes spécifiques pour DirectX, appelées lors
	// de la création/destruction/perte du device Dx.
	virtual void InitDxData();		// Initialise les données Dx
	virtual void ClearDxData();		// Libère les données Dx


protected:

	//crc32				m_TextureID;	// ID de la texture
	std::string			m_TextureName;	// Nom de la texture

	bool				m_IsDxReady;	// Si les données Dx sont initialisées

	LPD3DXSPRITE		m_pDxSprite;	// Sprite Dx
	LPDIRECT3DTEXTURE9	m_pDxTexture;	// Texture Dx

};

//******************************************************************
#endif		//_Sprite_H
