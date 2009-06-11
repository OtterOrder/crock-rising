#ifndef		_AIEnemy_H
#define		_AIEnemy_H

#include	<d3dx9.h>
#include	"AStar.h"
#include	"AIRess.h"
#include	"iostream"

class AIEnemy
{
public:
	AIEnemy(	unsigned int id, unsigned int	idGroup, unsigned int	idMember,	
				unsigned int hps, Vector3f pos, Vector3f dir );
	~AIEnemy(void);

	void enemyAIAttack();
	void enemyAIMoveTo( Vector3f posPlayer );
	void enemyAIEvade( Vector3f newPos );
	void enemyAIPatrol( Vector3f newPos );

	void setElapsedTime( float newElaTime )	{	elapsedTime = newElaTime;	}

public:
	unsigned int	idEnnemy;
	unsigned int	hpsEnemy;
	Vector3f		positEnnemy;
	Vector3f		direcEnnemy;
	Vector3f		transEnnemy;

	int posXScale;
	int posYScale;
	int posEXScale;
	int posEYScale;
	bool existPath;

	float	timeBetFindPath;
	float	elapsedTime;
	pair<int,int> nextWayPoint;

protected:
	unsigned int	idGroupEnnemy;
	unsigned int	idMemberEnnemy;

	AStar aPath;
};


#endif	// _AIEnemy_H