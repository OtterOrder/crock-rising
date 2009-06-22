#ifndef		_LevelMainMenu_H_
#define		_LevelMainMenu_H_

//******************************************************************

#include	<Game/Level.h>
#include	"../2D/ButtonText.h"

//******************************************************************

#define		LEVEL_mainmenu			0x56b55f63

//******************************************************************

class Camera;
class Sprite;
class SoundObject;

class LevelMainMenu : public Level
{
public:

	enum Link
	{
		NONE = -1,
		PLAY = 0,
		HIGHSCORES,
		SETTINGS,
		QUIT,

		NB_LINKS
	};

	LevelMainMenu				( crc32 levelID );			// Constructeur
	virtual ~LevelMainMenu		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:

	// Boutons du menu principal
	struct MMButton : public ButtonText
	{
		Link linkID;

		MMButton( Link _linkID );
		void OnClic();
	};
	
	Camera			*m_Camera;
	Sprite			*m_Background;
	MMButton		*m_Menu[NB_LINKS];
	SoundObject		*m_Sound;

};

//******************************************************************
#endif		//_LevelMainMenu_H_
