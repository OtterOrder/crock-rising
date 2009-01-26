#ifndef		_Sprite_H
#define		_Sprite_H

//******************************************************************

#include	"Object2D.h"

//******************************************************************

class Sprite : public Object2D
{
public:

	Sprite					( crc32 spriteID );
	virtual ~Sprite			( void );

	virtual void	Draw	( void ) const;		// Affiche l'objet


protected:

	Vector2f		m_Size;			// Taille du sprite
	
	crc32			m_TextureID;	// ID de la texture
	RECT			m_Rect;			// Coordonn�es de texture
	D3DCOLOR		m_Color;		// ?

	LPD3DXSPRITE	m_pSprite;		//..

};

//******************************************************************
#endif		//_Sprite_H