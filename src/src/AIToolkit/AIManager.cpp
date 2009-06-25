#include "AIManager.h"


AIManager::AIManager( bool spawn, int comportementAI, int fovEnemy, int rangeAttack, int scaleMap, int precision ) 
	:	nbGroup(0), spawnInfini(spawn), typeAI(comportementAI), cptGeneral(0), newPos(Vector3f(0,0,0)), 
		newAngle(0), fieldOfView(fovEnemy), attackRange(rangeAttack)
{
	aiEnemy = new AIEnemy(scaleMap, precision);
}

AIManager::~AIManager()
{
}

void AIManager::update( Vector3f posPlayer, float elapsedTime, vector<Enemy*> listEnemy )
{
	aiEnemy->setElapsedTime(elapsedTime);

	/////////////////////////////////////////////
	// Gère les états et transitions
	cptGeneral = 0;
	for ( vector<Enemy*>::iterator it = listAIEnemy->listEnemy.begin(); it != listAIEnemy->listEnemy.end(); )
	{
		// Calcul la distance entre le joueur et l'ennemi pour en déduire son état
		newPos = listEnemy[cptGeneral]->getSceneObjectAnimated()->GetPosition();
		distance = aiEnemy->calculDistance( posPlayer, newPos );
		newPos = Vector3f(0,0,0);
		newAngle = 0;

		if ( distance <= attackRange )		//**** TODO **** Rajouter champ de vision ****
		{
			if (listEnemy[cptGeneral]->getLife() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
			{
				aiEnemy->enemyAIAttack( posPlayer, listEnemy[cptGeneral]->getSceneObjectAnimated()->GetPosition(), newAngle );
			}
			else
			{
				aiEnemy->enemyAIEvade( posPlayer );
			}
		}
		else if ( distance <= fieldOfView )
		{
			if (listEnemy[cptGeneral]->getLife() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
			{
				aiEnemy->enemyAIMoveTo( posPlayer, listEnemy[cptGeneral]->getSceneObjectAnimated()->GetPosition(), newPos, newAngle);
			}
			else
			{
				aiEnemy->enemyAIEvade( posPlayer );
			}
		}
		else
		{
			aiEnemy->enemyAIPatrol(posPlayer);
		}

		listEnemy[cptGeneral]->getSceneObjectAnimated()->SetTranslation(newPos.x, 0, newPos.z);
		listEnemy[cptGeneral]->getSceneObjectAnimated()->SetRotation(0, (float)newAngle, 0);

		cptGeneral++;
		it++;
	}
}

void AIManager::spawn( int nbSpawn )
{
}