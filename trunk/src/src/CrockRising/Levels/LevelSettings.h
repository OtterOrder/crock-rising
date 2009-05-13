#ifndef		_LevelSettings_H_
#define		_LevelSettings_H_

//******************************************************************

#include	<Game/Level.h>

//******************************************************************

#define		LEVEL_settings			0xe545a0c5

//******************************************************************

class Camera;

class LevelSettings : public Level
{
public:

	LevelSettings				( crc32 levelID );			// Constructeur
	virtual ~LevelSettings		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera	*m_pCamera;

};

//******************************************************************
#endif		//_LevelSettings_H_
