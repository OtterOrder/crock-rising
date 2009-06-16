#include "AIManager.h"


AIManager::AIManager( bool spawn, int comportementAI ) 
	:	nbGroup(0), spawnInfini(spawn), typeAI(comportementAI), cptGeneral(0), 
		newPos(Vector3f(0,0,0)), newAngle(0)
{
}

AIManager::~AIManager()
{
}

void AIManager::update( Vector3f posPlayer, float elapsedTime, vector<Enemy*> listEnemy )
{	
	aiEnemy.setElapsedTime(elapsedTime);

	/////////////////////////////////////////////
	// Gère les états et transitions
	//vector<Enemy*>::iterator it = listAIEnemy->listEnemy.begin(); it != listAIEnemy->listEnemy.end();
	for ( cptGeneral=0; cptGeneral<4; cptGeneral++ )
	{
		// Calcul la distance entre le joueur et l'ennemi pour en déduire son état
		newPos = listEnemy[cptGeneral]->getSceneObjectAnimated()->GetPosition();
		distance = aiEnemy.calculDistance( posPlayer, newPos );
		newPos = Vector3f(0,0,0);
		newAngle = 0;

		if ( distance <= DISTANCE_ATTACK )		//**** TODO **** Rajouter champ de vision ****
		{
			if (listEnemy[cptGeneral]->getLife() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
			{
				aiEnemy.enemyAIAttack();
			}
			else
			{
				aiEnemy.enemyAIEvade( posPlayer );
			}
		}
		else if ( distance <= FIELD_OF_VIEW )
		{
			if (listEnemy[cptGeneral]->getLife() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
			{
				aiEnemy.enemyAIMoveTo( posPlayer, listEnemy[cptGeneral]->getSceneObjectAnimated()->GetPosition(), newPos, newAngle);
			}
			else
			{
				aiEnemy.enemyAIEvade( posPlayer );
			}
		}
		else
		{
			aiEnemy.enemyAIPatrol(posPlayer);
		}

		listEnemy[cptGeneral]->getSceneObjectAnimated()->SetTranslation(newPos.x, 0, newPos.z);
		listEnemy[cptGeneral]->getSceneObjectAnimated()->SetRotation(0, 0, newAngle);

		//cptGeneral++;
		//it++;
	}
}

void AIManager::spawn( int nbSpawn )
{
}