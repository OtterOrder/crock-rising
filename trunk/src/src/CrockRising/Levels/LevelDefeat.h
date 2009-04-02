#ifndef		_LevelDefeat_H_
#define		_LevelDefeat_H_

//******************************************************************

#include	<Game/Level.h>

//******************************************************************

#define		LEVEL_defeat			0x8aa5a05

//******************************************************************

class Camera;

class LevelDefeat : public Level
{
public:

	LevelDefeat					( crc32 levelID );			// Constructeur
	virtual ~LevelDefeat		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera	*m_pCamera;

};

//******************************************************************
#endif		//_LevelDefeat_H_
