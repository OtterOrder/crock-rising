#ifndef		_QUAD_H_
#define		_QUAD_H_

//******************************************************************

#include	"SceneObject2D.h"

//******************************************************************

class Quad : public SceneObject2D
{
public:

	Quad( u32 width, u32 height, const Color4f &color );
	Quad( const Vector2i &size, const Color4f &color );
	virtual ~Quad();

	// Affichage
	virtual void Draw();

	// Taille
	void SetSize( u32 width, u32 height );
	void SetSize( const Vector2i &size );
	u32 GetWidth() const;
	u32 GetHeight() const;

	// Taille relative aux dimensions de la fenêtre
	void SetRelSize( float relWidth, float relHeight );
	void SetRelSize( const Vector2f &relSize );
	float GetRelWidth() const;
	float GetRelHeight() const;

	// Gestion des données DirectX
	// Pas besoin..

protected:

	u32		m_Width;		// Largeur
	u32		m_Height;		// Hauteur

	void CommonInit();				// Initialisation commune aux constructeurs
	virtual void dirty_Refresh();	// Met à jour tout le bazar

};

//******************************************************************
#endif		//_QUAD_H_
