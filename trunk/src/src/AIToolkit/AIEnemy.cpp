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

	posPlayer2D_X = (int)floor(posPlayer.x*36.0f/500.0f);
	posPlayer2D_Y = (int)floor(posPlayer.z*36.0f/500.0f);
	posEnemy2D_X = (int)floor(positEnnemy.x*36.0f/500.0f);
	posEnemy2D_Y = (int)floor(positEnnemy.z*36.0f/500.0f);

	// Execute le pathfinding toutes les 400 milliSecondes pour optimiser
	if (timeBetFindPath > 50.0f)
	{
		nextWayPoint = aPath.findWay( posEnemy2D_X, posEnemy2D_Y, posPlayer2D_X, posPlayer2D_Y );
		timeBetFindPath = 0;
		//std::cout << "1: " << nextWayPoint.first << " , " << nextWayPoint.second << std::endl;
	}
	else	timeBetFindPath += elapsedTime;
	
	if ( nextWayPoint.first != -1 )
	{
		float tmpPosEXScale	= floor((nextWayPoint.first*500.0f)/36.0f);
		float tmpPosEZScale	= floor((nextWayPoint.second*500.0f)/36.0f);

		//std::cout << "1: " << tmpPosEXScale << " , " << tmpPosEZScale << std::endl;
		//std::cout << "2: " << posPlayer.x << " , " << posPlayer.z << std::endl;
		
		Vector3f tmpPla = Vector3f( posEnemy.x, 0, posEnemy.z );
		Vector3f tmpDir = Vector3f( tmpPosEXScale, 0, tmpPosEZScale );
		
		direcEnnemy = tmpDir - tmpPla;
		transEnnemy = tmpDir - tmpPla;
		D3DXVec3Normalize(&transEnnemy, &transEnnemy);
		transEnnemy *= elapsedTime/10.0f;
		
		//std::cout << transEnnemy.x << std::endl;

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