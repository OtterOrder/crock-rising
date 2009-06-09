#include "members.h"

members::members( int idMembersAI, Vector3f pos ) : existPath(0), timeBetFindPath(0.0f)
{
	m_pEnemy = new Enemy();
	
	// Variable de base 
	idMembers = idMembersAI;
	hpRemaining = 100;
	stateMember = AIRess::ATTACK;
}

members::~members()
{
}

void members::membersAttack( Vector3f posPlayer )
{
}

void members::membersMoveTo( Vector3f posPlayer, float elapsedTime )
{
	Vector3f posEnemy = m_pEnemy->getSceneObjectAnimated()->GetPosition();
	//position = posEnemy;

	posXScale = (int)floor(posPlayer.x*36.0f/500.0f);
	posYScale = (int)floor(posPlayer.z*36.0f/500.0f);
	posEXScale = (int)floor(posEnemy.x*36.0f/500.0f);
	posEYScale = (int)floor(posEnemy.z*36.0f/500.0f);

	// Execute le pathfinding toutes les 400 milliSecondes pour optimiser
	if (timeBetFindPath > 0.4f)
	{
		nextWayPoint = aPath.findWay( posXScale, posYScale, posEXScale, posEYScale );
		timeBetFindPath = 0;
		
		//std::cout << nextWayPoint.first << " , " << nextWayPoint.second << std::endl;
	}
	else	timeBetFindPath += elapsedTime;
	
	if ( nextWayPoint.first != -1 )
	{
		float tmpPosEXScale	= floor((nextWayPoint.first*500.0f)/36.0f);
		float tmpPosEYScale	= floor((nextWayPoint.second*500.0f)/36.0f);
		Vector3f tmpDir = Vector3f( tmpPosEXScale, 0, tmpPosEYScale );
		
		Vector3f newPos = posPlayer - tmpDir;
		newPos *= elapsedTime;

		/*
		Vector3f newPos = posPlayer - position;
		newPos *= elapsedTime;
		*/

		D3DXMATRIX pos;
		D3DXMatrixTranslation(&pos, tmpDir.x, tmpDir.y, tmpDir.z);
		m_pEnemy->getSceneObjectAnimated()->SetTransform( &pos );

		//position += tmpDir;

		m_pEnemy->getSceneObjectAnimated()->GetPosition();
	}
}

void members::membersEvade()
{
	//std::cout << "MEMBERS : " << idMembers << " EVADE." << std::endl;
}

void members::membersPatrol()
{
	//std::cout << "MEMBERS : " << idMembers << " PATROL." << std::endl;

}