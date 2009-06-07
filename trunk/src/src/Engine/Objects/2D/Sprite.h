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

	// Gestion des donn�es DirectX
	virtual void InitDxData();		// Initialise les donn�es Dx
	virtual void ClearDxData();		// Lib�re les donn�es Dx
	virtual bool IsDxReady() const;	// V�rifie si les donn�es Dx sont pr�tes

protected:

	std::string		m_SpriteName;	// Nom de la texture
	Texture			*m_Texture;		// Texture

};

//******************************************************************
#endif		//_SPRITE_H_
