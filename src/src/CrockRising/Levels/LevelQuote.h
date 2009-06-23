#ifndef		_LEVEL_QUOTE_H_
#define		_LEVEL_QUOTE_H_

//******************************************************************

#include	<string>
#include	<Game/Level.h>

//******************************************************************

#define		LEVEL_quote			0x6b71cbf4

//******************************************************************

class Camera;
class Text;

class LevelQuote : public Level
{
public:

	LevelQuote					( crc32 levelID );			// Constructeur
	virtual ~LevelQuote			( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:

	enum TextID
	{
		QUOTE_LINE1,
		QUOTE_LINE2,
		QUOTE_LINE3,
		SIGNATURE,

		NB_TEXTS
	};
	
	Camera			*m_Camera;
	Text			*m_Quote;
	Text			*m_Signature;
	
	std::string		m_Texts[NB_TEXTS];
	int				m_CurrentText;
	int				m_CurrentChar;
	float			m_Timer;

};

//******************************************************************
#endif		_LEVEL_QUOTE_H_
