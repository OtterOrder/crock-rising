#ifndef		_SPRITE_H_
#define		_SPRITE_H_

//******************************************************************

#include	"Quad.h"

//******************************************************************

class Texture;

class Sprite : public Quad
{
public:

	Sprite( const std::string &spriteName );
	virtual ~Sprite();

	// Affichage
	virtual void Draw();

	// Taille
	//void SetSizeToDefault();

	// Gestion des entités DirectX
	virtual void OnCreateDevice();
	virtual void OnDestroyDevice();
	virtual bool IsDrawable() const;

protected:

	std::string		m_SpriteName;	// Nom de la texture
	Texture			*m_Texture;		// Texture

};

//******************************************************************
#endif		//_SPRITE_H_
