#ifndef		_LevelMainMenu_H_
#define		_LevelMainMenu_H_

//******************************************************************

#include	<Game/Level.h>

//******************************************************************

#define		LEVEL_mainmenu			0x56b55f63

//******************************************************************

class Camera;

class LevelMainMenu : public Level
{
public:

	LevelMainMenu				( crc32 levelID );			// Constructeur
	virtual ~LevelMainMenu		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera	*m_pCamera;

};

//******************************************************************
#endif		//_LevelMainMenu_H_
