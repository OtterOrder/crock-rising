#ifndef		_LevelVictory_H_
#define		_LevelVictory_H_

//******************************************************************

#include	<Game/Level.h>

//******************************************************************

#define		LEVEL_victory			0x70bb005a

//******************************************************************

class Camera;

class LevelVictory : public Level
{
public:

	LevelVictory				( crc32 levelID );			// Constructeur
	virtual ~LevelVictory		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera	*m_pCamera;

};

//******************************************************************
#endif		//_LevelVictory_H_
