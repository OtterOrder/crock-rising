#include "AIManager.h"


AIManager::AIManager( bool spawn, int comportementAI, int nbMaxEnemy, int fovEnemy, int rangeAttack, int scaleMap, int precision ) 
	:	nbGroup(0), spawnInfini(spawn), typeAI(comportementAI), cptGeneral(0), newPos(Vector3f(0,0,0)), 
		newAngle(0), fieldOfView(fovEnemy), attackRange(rangeAttack), nbEnemy(nbMaxEnemy), scaleCurrMap(scaleMap),
		precCurrMap(precision)
{
	aiEnemy = new AIEnemy(scaleMap, precision);
}


AIManager::~AIManager()
{
	if (aiEnemy)	delete aiEnemy;
}


void AIManager::update( Vector3f posPlayer, float elapsedTime, vector<Enemy*> listEnemy )
{
	// Pour calcul la distance que doit effectuer les ennemies en fct du temps
	aiEnemy->setElapsedTime(elapsedTime);

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

	// Gère les respawn
	updateSpawn();
}


void AIManager::updateSpawn()
{
	// Recrée les enemies si le spawn est infini et qu'on a pas atteind le nombre maximal d'ennemies
	if ( spawnInfini )
	{
		// Creer les ennemis manquants
		while (listAIEnemy->listEnemy.size() < nbEnemy)
		{
			// Position aléatoire par rapport a la AIMap
			pair<int,int> posSpawn = aiEnemy->getPtrAStar()->randomSpawn();

			// Transpose les coordonnées de la AI map en coordonnées de jeu
			float spawnX = floor(float((posSpawn.first*scaleCurrMap)/precCurrMap)-scaleCurrMap/2);
			float spawnZ = floor(float((posSpawn.second*scaleCurrMap)/precCurrMap)-scaleCurrMap/2);
		
			new Enemy( Vector3f(spawnX, 8.f, spawnZ) );
		}
	}
}