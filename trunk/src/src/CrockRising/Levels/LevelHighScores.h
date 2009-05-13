#ifndef		_LevelHighScores_H_
#define		_LevelHighScores_H_

//******************************************************************

#include	<Game/Level.h>

//******************************************************************

#define		LEVEL_highscores			0x59f26d0

//******************************************************************

class Camera;

class LevelHighScores : public Level
{
public:

	LevelHighScores				( crc32 levelID );			// Constructeur
	virtual ~LevelHighScores	( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera	*m_pCamera;

};

//******************************************************************
#endif		//_LevelHighScores_H_
