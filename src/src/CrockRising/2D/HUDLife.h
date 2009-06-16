#ifndef		_2D_HUDLIFE_H_
#define		_2D_HUDLIFE_H_

//******************************************************************

#include	<Objects/2D/Sprite.h>

//******************************************************************

class HUDLife : protected Sprite
{
public:

	HUDLife();
	~HUDLife();

	void Init();
	void SetPosition( s32 posX, s32 posY );
	void SetScale( float scale );

	void SetMaxLife( s32 maxLife );
	void SetLife( s32 life );
	u32 GetLife() const;

protected:

	s32		m_Life;			// Points de vie
	s32		m_MaxLife;		// Nb max de points de vie

	float	m_Scale;		// Echelle..
	Quad	*m_LifeBar;		// Barre le vie

	u32 GetBarWidth() const;	// Calcule la largeur de la barre correspondante à m_Life

};

//******************************************************************
#endif		//_2D_HUDLIFE_H_
