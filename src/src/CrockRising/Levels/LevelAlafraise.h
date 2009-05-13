#ifndef		_LevelAlafraise_H_
#define		_LevelAlafraise_H_

//******************************************************************

#include	<Game/Level.h>
#include	<Core/Math.h>

//******************************************************************

#define		LEVEL_alafraise			0xa77df32f

//******************************************************************

class Camera;
class Sprite;

class LevelAlafraise : public Level
{
public:

	LevelAlafraise				( crc32 levelID );			// Constructeur
	virtual ~LevelAlafraise		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Camera	*m_pCamera;
	Sprite	*m_pLogo;		// Logo à la fraise

};

//******************************************************************
#endif		//_LevelAlafraise_H_
