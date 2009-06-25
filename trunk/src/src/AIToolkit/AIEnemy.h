#ifndef		_AIEnemy_H
#define		_AIEnemy_H

#include	<Core/Types/Vector.h>
#include	<d3dx9.h>
#include	"AStar.h"
#include	"iostream"

// Cf Math.h
#define M_PI 3.14159265358979323846

//////////////
class AIEnemy
{
public:
	AIEnemy( int scaleMap, int precision );
	~AIEnemy(void);

	void enemyAIMoveTo( Vector3f posPlayer, Vector3f positEnemy, Vector3f &newPos, int &angleRot );
	void enemyAIAttack( Vector3f posPlayer, Vector3f positEnemy, int &angleRot );
	void enemyAIPatrol( Vector3f newPos );
	void enemyAIEvade( Vector3f newPos );

	void setElapsedTime( float newElaTime )	{	elapsedTime = newElaTime;	}
	int	calculDistance( Vector3f pos1, Vector3f pos2 );

public:
	float elapsedTime;

	Vector3f	transEnemy;

	int			posPlayer2D_X;
	int			posPlayer2D_Y;
	int			posEnemy2D_X;
	int			posEnemy2D_Y;
	bool		existPath;

	float			timeBetFindPath;
	pair<int,int>	nextWayPoint;

protected:
	unsigned int	idGroupEnnemy;
	unsigned int	idMemberEnnemy;

	Vector3f		oldDirecEnemy;
	Vector3f		crossAngle;
	Vector3f		direcEnemy;
	float			dotAngle;

	int scale;
	int preci;

	AStar aPath;
};


#endif	// _AIEnemy_H