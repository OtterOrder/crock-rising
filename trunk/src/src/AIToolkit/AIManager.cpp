#include "AIManager.h"


AIManager::AIManager( bool spawn, int comportementAI, int nbMaxEnemy, int fovEnemy, int rangeAttack, int scaleMap, int precision ) 
	:	nbGroup(0), spawnInfini(spawn), typeAI(comportementAI), newPos(Vector3f(0,0,0)), 
		newAngle(0), fieldOfView(fovEnemy), attackRange(rangeAttack), nbEnemy(nbMaxEnemy), scaleCurrMap(scaleMap),
		precCurrMap(precision), timeEnemy(0.f)
{
	srand( (unsigned)time(NULL) );
	aiEnemy = new AIEnemy(scaleMap, precision);
}


AIManager::~AIManager()
{
	if (aiEnemy)	delete aiEnemy;
	//if (listAIEnemy)delete listAIEnemy;	
	std::vector<Enemy*>::iterator it = listAIEnemy->listEnemy.begin();
	int pouet = 0;
	while ( pouet != listAIEnemy->listEnemy.size() )
	{	
		Enemy* plop = listAIEnemy->listEnemy.at(pouet);
		//listAIEnemy->listEnemy.erase(it);
		
		if(plop->getSceneObjectAnimated()) 
		{
			SceneObject::RefList.remove( plop->getSceneObjectAnimated() );
			delete plop->getSceneObjectAnimated();
		}

		if(plop->getArme()) 
		{
			SceneObject::RefList.remove( plop->getArme() );
			delete plop->getArme();
		}

		pouet++;
	}
	listAIEnemy->listEnemy.clear();
	listAIEnemy = NULL;

}


void AIManager::update( Hero* const pHero, float elapsedTime, vector<Enemy*> listEnemy )
{
	// Pour calcul la distance que doit effectuer les ennemies en fct du temps
	aiEnemy->setElapsedTime(elapsedTime);
	Vector3f posPlayer = pHero->getSceneObjectAnimated()->GetPosition();

	// G�re les �tats et transitions
	for ( vector<Enemy*>::iterator it = listAIEnemy->listEnemy.begin(); it != listAIEnemy->listEnemy.end(); it++)
	{
		Enemy* pEnemy = (*it);

		// Calcul la distance entre le joueur et l'ennemi pour en d�duire son �tat
		if(pEnemy->IsAlive())
		{
			SceneObjectAnimated* pObj = pEnemy->getSceneObjectAnimated();
			if(pObj)
			{
				newPos = pEnemy->getSceneObjectAnimated()->GetPosition();
				distance = aiEnemy->calculDistance( posPlayer, newPos );
				newPos = Vector3f(0,0,0);
				newAngle = 0;

				if ( distance <= attackRange )
				{
					if (pEnemy->Life() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
					{
						aiEnemy->enemyAIAttack( posPlayer, pEnemy->getSceneObjectAnimated()->GetPosition(), newAngle );
						pEnemy->changeState(ATTACK);
					}
					else
					{
						aiEnemy->enemyAIEvade( posPlayer );
						pEnemy->changeState(RUN);
					}
				}
				else if ( distance <= fieldOfView )
				{
					if (pEnemy->Life() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
					{
						aiEnemy->enemyAIMoveTo( posPlayer, pEnemy->getSceneObjectAnimated()->GetPosition(), newPos, newAngle);
						pEnemy->changeState(RUN);
					}
					else
					{
						aiEnemy->enemyAIEvade( posPlayer );
						pEnemy->changeState(RUN);
					}
				}
				else
				{
					timeEnemy = pEnemy->timeSinceLastPath;
					aiEnemy->enemyAIPatrol( posPlayer, newPos, newAngle, timeEnemy);
					pEnemy->changeState(RUN);
					pEnemy->timeSinceLastPath = timeEnemy;
				}

				//(*it)->getSceneObjectAnimated()->SetTranslation(newPos.x, 0, newPos.z);
				//(*it)->getSceneObjectAnimated()->SetRotation(0, (float)newAngle, 0);

				Vector3f oldPos = pEnemy->getSceneObjectAnimated()->GetPosition();
				pEnemy->getSceneObjectAnimated()->SetPosition( oldPos.x+newPos.x, 0, oldPos.z+newPos.z );
				pEnemy->getSceneObjectAnimated()->SetRotation( 0, newAngle, 0 );

				NxActor* a = physX::getActor( pEnemy->getArme()->getEmpActor() );
				if( a )
					a->clearBodyFlag( NX_BF_FROZEN_ROT_Y );

				pEnemy->getArme()->SetRotation( 0.f, -newAngle, 0.f );
				
				if( a )
					a->raiseBodyFlag( NX_BF_FROZEN_ROT_Y );
				a=NULL;

				pEnemy->update( listAIEnemy );
			}
		}
	}

	// G�re les respawn
	updateSpawn(pHero);
}


void AIManager::updateSpawn( Hero* const pHero )
{
	// Recr�e les enemies si le spawn est infini et qu'on a pas atteind le nombre maximal d'ennemies
	if ( spawnInfini )
	{
		// Creer les ennemis manquants
		while ((int)listAIEnemy->listEnemy.size() < nbEnemy)
		{
			// Position al�atoire par rapport a la AIMap
			pair<int,int> posSpawn = aiEnemy->getPtrAStar()->randomSpawn();

			// Transpose les coordonn�es de la AI map en coordonn�es de jeu
			float spawnX = floor(float((posSpawn.first*scaleCurrMap)/precCurrMap)-scaleCurrMap/2);
			float spawnZ = floor(float((posSpawn.second*scaleCurrMap)/precCurrMap)-scaleCurrMap/2);
		
			Enemy* enemy = NULL;
  			if ( rand()%2)		enemy = new Alien( Vector3f(spawnX, 8.f, spawnZ) );
   			else				enemy = new MmeGrise( Vector3f(spawnX, 8.f, spawnZ) );
			
			enemy->Init();
			enemy->getSceneObjectAnimated()->SetControledCharacter(3.f, 7.f, enemy );

			physX::Link( pHero->getArme(), enemy->getSceneObjectAnimated() );
			physX::Link( enemy->getArme(), pHero->getSceneObjectAnimated() );
		}
	}
}
