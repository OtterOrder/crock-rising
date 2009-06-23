#ifndef		_LevelHighScores_H_
#define		_LevelHighScores_H_

//******************************************************************

#include	<Game/Level.h>

//******************************************************************

#define		LEVEL_highscores			0x59f26d0

//******************************************************************

class Camera;
class Sprite;
class Quad;
class Text;
class SoundObject;
class BackButton;

class LevelHighScores : public Level
{
public:

	LevelHighScores				( crc32 levelID );			// Constructeur
	virtual ~LevelHighScores	( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:

	Camera			*m_Camera;
	Sprite			*m_Background;
	
	BackButton		*m_BackButton;
	SoundObject		*m_Sound;

	Quad			*m_ScoresBG;
	Text			*m_Scores;

};

//******************************************************************
#endif		//_LevelHighScores_H_
