#ifndef		_LevelGameMenu_H_
#define		_LevelGameMenu_H_

//******************************************************************

#include	<Game/Level.h>

//******************************************************************

#define		LEVEL_gamemenu			0x2b3430ac

//******************************************************************

class Camera;

class LevelGameMenu : public Level
{
public:

	LevelGameMenu				( crc32 levelID );			// Constructeur
	virtual ~LevelGameMenu		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera	*m_pCamera;

};

//******************************************************************
#endif		//_LevelGameMenu_H_
