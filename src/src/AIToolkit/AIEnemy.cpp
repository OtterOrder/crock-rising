#include "AIEnemy.h"

AIEnemy::AIEnemy(	unsigned int id, unsigned int idGroup, unsigned int	idMember,	
					unsigned int hps, Vector3f pos, Vector3f dir )	
	:	idEnnemy(id), idGroupEnnemy(idGroup), idMemberEnnemy(idMember), direcEnnemy(dir), 
		positEnnemy(pos), transEnnemy(Vector3f(0, 0, 0)), timeBetFindPath(0.f)
{	
}

AIEnemy::~AIEnemy(void)
{
}


void AIEnemy::enemyAIAttack()
{
}

void AIEnemy::enemyAIMoveTo( Vector3f posPlayer )
{
	Vector3f posEnemy = positEnnemy;

	posXScale = (int)floor(posPlayer.x*36.0f/500.0f);
	posYScale = (int)floor(posPlayer.z*36.0f/500.0f);
	posEXScale = (int)floor(positEnnemy.x*36.0f/500.0f);
	posEYScale = (int)floor(positEnnemy.z*36.0f/500.0f);

	// Execute le pathfinding toutes les 400 milliSecondes pour optimiser
	if (timeBetFindPath > 400.0f)
	{
		nextWayPoint = aPath.findWay( posXScale, posYScale, posEYScale, posEXScale );
		timeBetFindPath = 0;
		
		//std::cout << nextWayPoint.first << " , " << nextWayPoint.second << std::endl;
	}
	else	timeBetFindPath += elapsedTime;
	
	if ( nextWayPoint.first != -1 )
	{
		float tmpPosEXScale	= floor((nextWayPoint.first*500.0f)/36.0f);
		float tmpPosEZScale	= floor((nextWayPoint.second*500.0f)/36.0f);

		//std::cout << tmpPosEXScale << " , " << tmpPosEZScale << std::endl;
		//std::cout << posPlayer.x << " , " << posPlayer.z << std::endl;
		
		Vector3f tmpPla = Vector3f( posPlayer.x, 0, posPlayer.z );
		Vector3f tmpDir = Vector3f( tmpPosEXScale, 0, tmpPosEZScale );
		
		transEnnemy = tmpDir - tmpPla;
		//transEnnemy *= elapsedTime/100.f;

		positEnnemy += transEnnemy;

		//std::cout << positEnnemy.x << " , " << positEnnemy.y << " , " << positEnnemy.z << " , " << std::endl;
	}
}

void AIEnemy::enemyAIEvade( Vector3f newPos )
{
}

void AIEnemy::enemyAIPatrol( Vector3f newPos )
{
}