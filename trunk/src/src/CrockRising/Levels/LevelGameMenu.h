#ifndef		_LEVEL_GAMEMENU_H_
#define		_LEVEL_GAMEMENU_H_

//******************************************************************

#include	<Game/Level.h>
#include	"../2D/ButtonText.h"

//******************************************************************

#define		LEVEL_gamemenu			0x2b3430ac

//******************************************************************

class Camera;
class Sprite;
class SoundObject;

class LevelGameMenu : public Level
{
public:

	enum Link
	{
		NONE = -1,
		BACK = 0,

		NB_LINKS
	};

	LevelGameMenu				( crc32 levelID );			// Constructeur
	virtual ~LevelGameMenu		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:

	// Boutons du menu de jeu
	struct GMButton : public ButtonText
	{
		Link linkID;

		GMButton( Link _linkID );
		void OnClic();
	};
	
	Camera			*m_Camera;
	Sprite			*m_Background;
	GMButton		*m_Menu[NB_LINKS];
	SoundObject		*m_Sound;

};

//******************************************************************
#endif		//_LEVEL_GAMEMENU_H_
