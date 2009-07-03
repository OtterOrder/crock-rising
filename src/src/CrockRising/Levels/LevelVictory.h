#ifndef		_LevelVictory_H_
#define		_LevelVictory_H_

//******************************************************************

#include	<Game/Level.h>
#include	<Core/Math.h>

//******************************************************************

#define		LEVEL_victory			0x70bb005a

//******************************************************************

class Camera;
class Sprite;

class LevelVictory : public Level
{
public:

	LevelVictory				( crc32 levelID );			// Constructeur
	virtual ~LevelVictory		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau


protected:
	
	Sprite	*m_pLogo;		// Logo à la fraise
	float	m_LogoTimer;
	Camera	*m_pCamera;

};

//******************************************************************
#endif		//_LevelVictory_H_
