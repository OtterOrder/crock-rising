#ifndef		_2D_BUTTONSPRITE_H_
#define		_2D_BUTTONSPRITE_H_

//******************************************************************

#include	<Objects/2D/Sprite.h>

//******************************************************************

class SoundObject;

class ButtonSprite : public Sprite
{
public:

	enum State
	{
		WAIT,
		HOVER
	};

	ButtonSprite( const std::string &spriteName );
	virtual ~ButtonSprite() {}

	virtual void Update();
	
	// Action au clic. A surchager
	virtual void OnClic() {}

	void Activate( bool isActive );
	void SetWaitColor( const Color4f &color );
	void SetHoverColor( const Color4f &color );
	void SetSound( SoundObject *pSound );

	inline State GetState() const { return m_State; }

protected:

	State			m_State;
	bool			m_IsActive;
	Color4f			m_WaitColor;
	Color4f			m_HoverColor;
	SoundObject		*m_pSound;

	void SetState( State state );

};

//******************************************************************
#endif		//_2D_BUTTONSPRITE_H_
