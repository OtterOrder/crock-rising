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
	virtual void DrawWithoutEffect();

	// Taille
	void SetSize( u32 width, u32 height );
	void SetSize( const Vector2i &size );
	void SetWidth( u32 width );
	void SetHeight( u32 height );
	u32 GetWidth() const;
	u32 GetHeight() const;

	// Flags
	void SetAsBackground( bool background );

	// Collision
	virtual bool IsCollide( s32 posX, s32 posY );

	// Gestion des entités DirectX
	// ..

protected:

	u32		m_Width;		// Largeur
	u32		m_Height;		// Hauteur

	void CommonInit();				// Initialisation commune aux constructeurs
	virtual void dirty_Refresh();	// Met à jour tout le bazar

};

//******************************************************************
#endif		//_QUAD_H_
