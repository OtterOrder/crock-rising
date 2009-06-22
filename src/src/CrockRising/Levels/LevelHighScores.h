#ifndef		_LevelHighScores_H_
#define		_LevelHighScores_H_

//******************************************************************

#include	<Game/Level.h>
#include	"../2D/ButtonText.h"

//******************************************************************

#define		LEVEL_highscores			0x59f26d0

//******************************************************************

class Camera;
class Sprite;
class SoundObject;

class LevelHighScores : public Level
{
public:

	LevelHighScores				( crc32 levelID );			// Constructeur
	virtual ~LevelHighScores	( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:

	// Bouton retour
	struct BackButton : public ButtonText
	{
		BackButton();
		void OnClic();
	};
	
	Camera			*m_Camera;
	Sprite			*m_Background;
	BackButton		*m_BackButton;
	SoundObject		*m_Sound;

};

//******************************************************************
#endif		//_LevelHighScores_H_
