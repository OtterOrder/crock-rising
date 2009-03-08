#ifndef		_Sprite_H
#define		_Sprite_H

//******************************************************************

#include	"Quad.h"

//******************************************************************

class Sprite : public Quad
{
public:

	//-- M�thodes publiques
	
	//Sprite( crc32 spriteID );
	Sprite( const char *path );
	virtual ~Sprite();

	virtual void Draw();		// Affiche l'objet

	//-- M�thodes sp�cifiques pour DirectX (utilis�es dans le Renderer)
	
	virtual void InitDxData();		// Initialise les donn�es Dx
	virtual void ClearDxData();		// Lib�re les donn�es Dx


protected:

	//crc32				m_TextureID;	// ID de la texture
	std::string			m_TextureName;	// Nom de la texture

};

//******************************************************************
#endif		//_Sprite_H
