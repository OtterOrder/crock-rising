#ifndef		_AIEnemy_H
#define		_AIEnemy_H

#include	<Core/Types/Vector.h>
#include	<Physics/Boundingbox.h>
#include	<d3dx9.h>
#include	"AStar.h"
#include	"iostream"
#include	<stdlib.h>
#include	<time.h>
#include	<vector>
#include	<map>

#define M_2PI 6.28318530717958647692f
#define M_PI 3.14159265358979323846f
#define M_PI2 1.57079632679489661923f

//////////////
class AIEnemy
{
public:
	AIEnemy( int scaleMap, int precision );
	~AIEnemy(void);

	void enemyAIMoveTo( Vector3f posPlayer, Vector3f positEnemy, Vector3f &newPos, float &angleRot );
	void enemyAIAttack( Vector3f posPlayer, Vector3f positEnemy, float &angleRot );
	void enemyAIPatrol( Vector3f positEnemy, Vector3f &newPos, float &angleRot, float &time );
	void enemyAIEvade( Vector3f newPos );

	void setElapsedTime( float newElaTime )	{	elapsedTime = newElaTime;	}
	int	calculDistance( Vector3f pos1, Vector3f pos2 );

	AStar* getPtrAStar()	{	return aPath;	}

public:
	float elapsedTime;

	Vector3f		transEnemy;

	int				posPlayer2D_X;
	int				posPlayer2D_Y;
	int				posEnemy2D_X;
	int				posEnemy2D_Y;
	int				posPatrol2D_X;
	int				posPatrol2D_Y;
	bool			existPath;

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

	AStar* aPath;
};


#endif	// _AIEnemy_H