#ifndef		_Sprite_H
#define		_Sprite_H

//******************************************************************

#include	"Quad.h"

//******************************************************************

class Texture;

class Sprite : public Quad
{
public:

	//-- Méthodes publiques
	
	//Sprite( crc32 spriteID );
	Sprite( const char *path );
	virtual ~Sprite();

	virtual void Draw();		// Affiche l'objet

	//-- Méthodes spécifiques pour DirectX
	
	virtual void InitDxData();		// Initialise les données Dx
	virtual void ClearDxData();		// Libère les données Dx
	virtual bool IsDxReady() const;	// Vérifie si les données Dx sont prètes

protected:

	//crc32				m_TextureID;	// ID de la texture
	std::string			m_TextureName;	// Nom de la texture
	Texture				*m_pTexture;	// Texture

};

//******************************************************************
#endif		//_Sprite_H
