#ifndef		_LEVEL_CANYON_H_
#define		_LEVEL_CANYON_H_

//******************************************************************

#include	<Game/Level.h>

//******************************************************************

#define		LEVEL_canyon			0x53466a72

//******************************************************************

class Camera;

class LevelCanyon : public Level
{
public:

	LevelCanyon					( crc32 levelID );			// Constructeur
	virtual ~LevelCanyon		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera	*m_Camera;

};

//******************************************************************
#endif		//_LEVEL_CANYON_H_
