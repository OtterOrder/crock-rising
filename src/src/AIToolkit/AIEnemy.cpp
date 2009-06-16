#include "AIEnemy.h"

AIEnemy::AIEnemy()
	:	direcEnemy(Vector3f(0, 0, 0)), transEnemy(Vector3f(0, 0, 0)), timeBetFindPath(0.f), 
		oldDirecEnemy(Vector3f(0, 0, 0)), dotAngle(0.f), angleRotEnemy(0)
{
}

AIEnemy::~AIEnemy(void)
{
}

void AIEnemy::enemyAIAttack()
{
}

void AIEnemy::enemyAIMoveTo( Vector3f posPlayer, Vector3f positEnemy, Vector3f &newPos, int &angleRot )
{	
	posPlayer2D_X = (int)floor(posPlayer.x*36.0f/500.0f);
	posPlayer2D_Y = (int)floor(posPlayer.z*36.0f/500.0f);
	posEnemy2D_X = (int)floor(positEnemy.x*36.0f/500.0f);
	posEnemy2D_Y = (int)floor(positEnemy.z*36.0f/500.0f);

	// Execute le pathfinding toutes les 50 milliSecondes pour optimiser (pathfind couteux en calcul)
	// ATTENTION : MiniBlocage si l'ennemi arrive avant que le nouveau point soit calculé.
	//if (timeBetFindPath > 50.0f)
	//{
		nextWayPoint = aPath.findWay( posEnemy2D_X, posEnemy2D_Y, posPlayer2D_X, posPlayer2D_Y );
		timeBetFindPath = 0;
	//}
	//else	
		timeBetFindPath += elapsedTime;

	// Si il a trouvé un chemin
	if ( nextWayPoint.first != -1 )
	{
		// Calcul les vecteurs de direction
		float tmpPosEXScale	= floor((nextWayPoint.first*500.0f)/36.0f);
		float tmpPosEZScale	= floor((nextWayPoint.second*500.0f)/36.0f);
		
		Vector3f tmpPla = Vector3f( positEnemy.x, 0, positEnemy.z );
		Vector3f tmpDir = Vector3f( tmpPosEXScale, 0, tmpPosEZScale );
		
		// Calcul l'angle pour la rotation de l'ennemi
		dotAngle = 0.f;
		angleRotEnemy = 0;
		oldDirecEnemy = direcEnemy;
		direcEnemy = tmpDir - tmpPla;
		D3DXVec3Normalize(&oldDirecEnemy, &oldDirecEnemy);
		D3DXVec3Normalize(&direcEnemy, &direcEnemy);
		dotAngle = D3DXVec3Dot(&direcEnemy, &oldDirecEnemy);
		//std::cout << "OldDir : " << oldDirecEnemy.x << " , "<< oldDirecEnemy.y << " , " << oldDirecEnemy.z << std::endl;
		//std::cout << "Dir : " << direcEnemy.x << " , "<< direcEnemy.y << " , " << direcEnemy.z << std::endl;
		D3DXVec3Cross(&crossAngle, &direcEnemy, &oldDirecEnemy );

		if (dotAngle > 0.001f && dotAngle < 0.9999f)
		{
			angleRot = D3DXToDegree(acos(dotAngle));
			if ( crossAngle.y < 0 )		angleRot = -angleRotEnemy;
		}

		// Calcul le déplacement de l'ennemi
		newPos = tmpDir - tmpPla;
		D3DXVec3Normalize(&newPos, &newPos);
		newPos *= elapsedTime/30.0f;
		positEnemy += newPos;
	}
}

void AIEnemy::enemyAIEvade( Vector3f newPos )
{
}

void AIEnemy::enemyAIPatrol( Vector3f newPos )
{
}

int AIEnemy::calculDistance( Vector3f pos1, Vector3f pos2 )
{
	return (int)(sqrt(pow((pos2.x-pos1.x),2) + pow((pos2.y-pos1.y),2) + pow((pos2.z-pos1.z),2)));
}