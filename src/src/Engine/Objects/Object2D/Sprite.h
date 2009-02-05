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

	// Transformations d'échelle
	void SetScale( float scaleX, float scaleY );	// Change l'échelle
	void SetScale( const Vector2f &scale );			// Change l'échelle
	Vector2f GetScale() const;						// Donne l'échelle

	// Alpha
	void SetAlpha( float alpha );	// Change l'alpha (0->1)
	float GetAlpha() const;			// Donne l'alpha

	// Méthodes spécifiques pour DirectX, appelées lors
	// de la création/destruction/perte du device Dx.
	virtual void InitDxData();		// Initialise les données Dx
	virtual void ClearDxData();		// Libère les données Dx


protected:

	Vector3f			m_Scale;		// Echelle
	float				m_Alpha;		// Alpha
	
	//crc32				m_TextureID;	// ID de la texture
	std::string			m_TextureName;	// Nom de la texture
	
	bool				m_IsDxReady;	// Si les données Dx sont initialisées
	LPD3DXSPRITE		m_pDxSprite;	// Sprite Dx
	LPDIRECT3DTEXTURE9	m_pDxTexture;	// Texture Dx

private:

	// =========================================================
	// Méthodes privées
	
	void CommonInit();	// Initialisation commune aux constructeurs

};

//******************************************************************
#endif		//_Sprite_H
