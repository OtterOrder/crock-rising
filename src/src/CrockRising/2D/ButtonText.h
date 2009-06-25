#ifndef		_2D_BUTTONTEXT_H_
#define		_2D_BUTTONTEXT_H_

//******************************************************************

#include	<Objects/2D/Text.h>

//******************************************************************

class SoundObject;

class ButtonText : public Text
{
public:

	enum State
	{
		WAIT,
		HOVER
	};

	ButtonText( const std::string &text = "" );
	ButtonText( const std::string &text, const std::string &fontName, u32 size, bool bold, bool italic );
	virtual ~ButtonText() {}

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

	void CommonInit();
	void SetState( State state );

};

//******************************************************************

// Bouton retour au mainmenu générique
class BackButton : public ButtonText
{
public:

	BackButton();
	void OnClic();

};

//******************************************************************
#endif		//_2D_BUTTONTEXT_H_
