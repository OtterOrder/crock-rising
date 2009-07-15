#include "AIEnemy.h"


AIEnemy::AIEnemy( int scaleMap, int precision )
	:	direcEnemy(Vector3f(0, 0, 0)), transEnemy(Vector3f(0, 0, 0)), timeBetFindPath(0.f),
		oldDirecEnemy(Vector3f(0, 0, 0)), dotAngle(0.f), scale(scaleMap), preci(precision)
{
	srand( (unsigned)time(NULL) );
	aPath = new AStar();
}

AIEnemy::~AIEnemy(void)
{
	if(aPath)	delete aPath;
}

void AIEnemy::enemyAIAttack( Vector3f posPlayer, Vector3f positEnemy, float &angleRot )
{
	//Rotation
	Vector3f posEnnemy = Vector3f( positEnemy.x, 0, positEnemy.z );
	Vector3f posTmp = Vector3f( posPlayer.x, 0, posPlayer.z );
	Vector3f dirVEnnemy = posTmp - posEnnemy;

	D3DXVec3Normalize(&dirVEnnemy, &dirVEnnemy);
	D3DXVECTOR3 axeX( 1, 0, 0 ), axeZ( 0, 0, 1 );
	dotAngle = D3DXVec3Dot( &dirVEnnemy, &axeX );
	angleRot = (dotAngle - 1) * M_PI2 + M_PI2;

	float dotAngle2 = D3DXVec3Dot( &dirVEnnemy, &axeZ );
	if( dotAngle2 > 0.f ) angleRot = M_PI - angleRot;

	/*static NxActor* a = physX::getActor( pEnemy->getArme()->getEmpActor() );
	if( a )
		a->clearBodyFlag( NX_BF_FROZEN_ROT_Y );

	pEnemy->getArme()->SetRotation( 0, -D3DXToDegree( angleRot ), 0 );
	
	if( a )
		a->raiseBodyFlag( NX_BF_FROZEN_ROT_Y );*/
}

void AIEnemy::enemyAIMoveTo( Vector3f posPlayer, Vector3f positEnemy, Vector3f &newPos, float &angleRot )
{
	posPlayer2D_X = (int)floor((posPlayer.x*preci/scale)+preci/2);
	posPlayer2D_Y = (int)floor((posPlayer.z*preci/scale)+preci/2);
	posEnemy2D_X = (int)floor((positEnemy.x*preci/scale)+preci/2);
	posEnemy2D_Y =(int) floor((positEnemy.z*preci/scale)+preci/2);

	nextWayPoint = aPath->findWay( posEnemy2D_X, posEnemy2D_Y, posPlayer2D_X, posPlayer2D_Y );

	// Si il a trouvé un chemin
	if ( nextWayPoint.first != -1 )
	{
		// Calcul les vecteurs de direction
		float tmpPosEXScale	= floor(float((nextWayPoint.first*scale)/preci)-scale/2);
		float tmpPosEZScale	= floor(float((nextWayPoint.second*scale)/preci)-scale/2);
		
		Vector3f tmpPla = Vector3f( positEnemy.x, 0, positEnemy.z );
		Vector3f tmpDir = Vector3f( tmpPosEXScale, 0, tmpPosEZScale );

		// Rotation
		Vector3f posEnnemy = Vector3f( (float)posEnemy2D_X, 0, (float)posEnemy2D_Y );
		Vector3f posPlayer2 = Vector3f( (float)posPlayer2D_X, 0, (float)posPlayer2D_Y );
		Vector3f dirVEnnemy = posPlayer2 - posEnnemy;

		D3DXVec3Normalize(&dirVEnnemy, &dirVEnnemy);
		D3DXVECTOR3 axeX( 1, 0, 0 ), axeZ( 0, 0, 1 );
		dotAngle = D3DXVec3Dot( &dirVEnnemy, &axeX );
		angleRot = (dotAngle - 1) * M_PI2 + M_PI2;

		float dotAngle2 = D3DXVec3Dot( &dirVEnnemy, &axeZ );
		if( dotAngle2 > 0.f ) angleRot = M_PI - angleRot;

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

void AIEnemy::enemyAIPatrol( Vector3f positEnemy, Vector3f &newPos, float &angleRot, float &time )
{
	if (time > 3000.0f)
	{
		posEnemy2D_X = (int)floor((positEnemy.x*preci/scale)+preci/2);
		posEnemy2D_Y = (int)floor((positEnemy.z*preci/scale)+preci/2);
		posPatrol2D_X = posEnemy2D_X;
		posPatrol2D_Y = posEnemy2D_Y;

		switch( rand()%4 )
		{
		case 0:		posPatrol2D_X += 6;	posPatrol2D_Y += 6;	break;
		case 1:		posPatrol2D_X += 6;	posPatrol2D_Y -= 6;	break;
		case 2: 	posPatrol2D_X -= 6;	posPatrol2D_Y += 6;	break;
		case 3: 	posPatrol2D_X -= 6;	posPatrol2D_Y -= 6;	break;
		}

		nextWayPoint = aPath->findWay( posEnemy2D_X, posEnemy2D_Y, posPatrol2D_X, posPatrol2D_Y );
		time = 0.0f;
	}
	else
	{
		time += elapsedTime;
	}

	// Si il a trouvé un chemin
	if ( nextWayPoint.first != -1 )
	{
		// Calcul les vecteurs de direction
		float tmpPosEXScale	= floor(float((nextWayPoint.first*scale)/preci)-scale/2);
		float tmpPosEZScale	= floor(float((nextWayPoint.second*scale)/preci)-scale/2);
		
		Vector3f tmpPla = Vector3f( positEnemy.x, 0, positEnemy.z );
		Vector3f tmpDir = Vector3f( tmpPosEXScale, 0, tmpPosEZScale );
		
		// Calcul le déplacement de l'ennemi
		newPos = tmpDir - tmpPla;
		D3DXVec3Normalize(&newPos, &newPos);
		newPos *= elapsedTime/100.0f;
		positEnemy += newPos;
	}
}

int AIEnemy::calculDistance( Vector3f pos1, Vector3f pos2 )
{
	return (int)(sqrt(pow((pos2.x-pos1.x),2) + pow((pos2.y-pos1.y),2) + pow((pos2.z-pos1.z),2)));
}