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

	// Transformations d'�chelle
	void SetScale( float scaleX, float scaleY );	// Change l'�chelle
	void SetScale( const Vector2f &scale );			// Change l'�chelle
	Vector2f GetScale() const;						// Donne l'�chelle

	// Alpha
	void SetAlpha( float alpha );	// Change l'alpha (0->1)
	float GetAlpha() const;			// Donne l'alpha

	// M�thodes sp�cifiques pour DirectX, appel�es lors
	// de la cr�ation/destruction/perte du device Dx.
	virtual void InitDxData();		// Initialise les donn�es Dx
	virtual void ClearDxData();		// Lib�re les donn�es Dx


protected:

	Vector3f			m_Scale;		// Echelle
	float				m_Alpha;		// Alpha
	
	//crc32				m_TextureID;	// ID de la texture
	std::string			m_TextureName;	// Nom de la texture
	
	bool				m_IsDxReady;	// Si les donn�es Dx sont initialis�es
	LPD3DXSPRITE		m_pDxSprite;	// Sprite Dx
	LPDIRECT3DTEXTURE9	m_pDxTexture;	// Texture Dx

private:

	// =========================================================
	// M�thodes priv�es
	
	void CommonInit();	// Initialisation commune aux constructeurs

};

//******************************************************************
#endif		//_Sprite_H
