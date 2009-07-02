#include "AIManager.h"


AIManager::AIManager( bool spawn, int comportementAI, int nbMaxEnemy, int fovEnemy, int rangeAttack, int scaleMap, int precision ) 
	:	nbGroup(0), spawnInfini(spawn), typeAI(comportementAI), newPos(Vector3f(0,0,0)), 
		newAngle(0), fieldOfView(fovEnemy), attackRange(rangeAttack), nbEnemy(nbMaxEnemy), scaleCurrMap(scaleMap),
		precCurrMap(precision)
{
	srand( (unsigned)time(NULL) );
	aiEnemy = new AIEnemy(scaleMap, precision);
}


AIManager::~AIManager()
{
	if (aiEnemy)	delete aiEnemy;
}


void AIManager::update( Hero* const pHero, float elapsedTime, vector<Enemy*> listEnemy )
{
	// Pour calcul la distance que doit effectuer les ennemies en fct du temps
	aiEnemy->setElapsedTime(elapsedTime);
	Vector3f posPlayer = pHero->getSceneObjectAnimated()->GetPosition();

	// Gère les états et transitions
	for ( vector<Enemy*>::iterator it = listAIEnemy->listEnemy.begin(); it != listAIEnemy->listEnemy.end(); it++)
	{
		// Calcul la distance entre le joueur et l'ennemi pour en déduire son état
		if((*it)->IsAlive())
		{
			SceneObjectAnimated* pObj = (*it)->getSceneObjectAnimated();
			if(pObj)
			{
				newPos = (*it)->getSceneObjectAnimated()->GetPosition();
				distance = aiEnemy->calculDistance( posPlayer, newPos );
				newPos = Vector3f(0,0,0);
				newAngle = 0;

				if ( distance <= attackRange )
				{
					if ((*it)->Life() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
					{
						aiEnemy->enemyAIAttack( posPlayer, (*it)->getSceneObjectAnimated()->GetPosition(), newAngle );
						(*it)->changeState(ATTACK);
					}
					else
					{
						aiEnemy->enemyAIEvade( posPlayer );
						(*it)->changeState(RUN);
					}
				}
				else if ( distance <= fieldOfView )
				{
					if ((*it)->Life() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
					{
						aiEnemy->enemyAIMoveTo( posPlayer, (*it)->getSceneObjectAnimated()->GetPosition(), newPos, newAngle);
						(*it)->changeState(RUN);
					}
					else
					{
						aiEnemy->enemyAIEvade( posPlayer );
						(*it)->changeState(RUN);
					}
				}
				else
				{
					aiEnemy->enemyAIPatrol(posPlayer);
					(*it)->changeState(RUN);
				}

				(*it)->getSceneObjectAnimated()->SetTranslation(newPos.x, 0, newPos.z);
				(*it)->getSceneObjectAnimated()->SetRotation(0, (float)newAngle, 0);

				(*it)->update( listAIEnemy );
			}
		}
	}

	// Gère les respawn
	updateSpawn(pHero);
}


void AIManager::updateSpawn( Hero* const pHero )
{
	// Recrée les enemies si le spawn est infini et qu'on a pas atteind le nombre maximal d'ennemies
	if ( spawnInfini )
	{
		// Creer les ennemis manquants
		while ((int)listAIEnemy->listEnemy.size() < nbEnemy)
		{
			// Position aléatoire par rapport a la AIMap
			pair<int,int> posSpawn = aiEnemy->getPtrAStar()->randomSpawn();

			// Transpose les coordonnées de la AI map en coordonnées de jeu
			float spawnX = floor(float((posSpawn.first*scaleCurrMap)/precCurrMap)-scaleCurrMap/2);
			float spawnZ = floor(float((posSpawn.second*scaleCurrMap)/precCurrMap)-scaleCurrMap/2);
		
			Enemy* enemy = NULL;
  			if ( rand()%2)
  				enemy = new Alien( Vector3f(spawnX, 8.f, spawnZ) );
   			else
 				enemy = new MmeGrise( Vector3f(spawnX, 8.f, spawnZ) );
			
			enemy->Init();
			enemy->getSceneObjectAnimated()->SetControledCharacter(3.f, 7.f, enemy );

			physX::Link( pHero->getArme(), enemy->getSceneObjectAnimated() );
		}
	}
}
