#include "AIManager.h"
#include "../Engine/Core/Macros.h"


AIManager::AIManager( bool spawn, int comportementAI, int nbMaxEnemy, int fovEnemy, int rangeAttack, int scaleMap, int precision ) 
	:	nbGroup(0), spawnInfini(spawn), typeAI(comportementAI), newPos(Vector3f(0,0,0)), 
		newAngle(0), fieldOfView(fovEnemy), attackRange(rangeAttack), nbEnemy(nbMaxEnemy), scaleCurrMap(scaleMap),
		precCurrMap(precision), timeEnemy(0.f)
{
	initRandom();
	aiEnemy = new AIEnemy(scaleMap, precision);

}


AIManager::~AIManager()
{
	if (aiEnemy)	delete aiEnemy;
	std::list<Enemy*>::iterator it = Enemy::RefList.begin();
	while( !Enemy::RefList.empty() )
	{
		Enemy* enemy = Enemy::RefList.back() ;
		releaseFromList( Enemy::RefList, enemy);
	}
}


void AIManager::update( Hero* const pHero, float elapsedTime )
{
	// Pour calcul la distance que doit effectuer les ennemies en fct du temps
	aiEnemy->setElapsedTime(elapsedTime);
	Vector3f posPlayer = pHero->getSceneObjectAnimated()->GetPosition();

	// Gère les états et transitions
	list<Enemy*>::iterator it = Enemy::RefList.begin();
	while( it != Enemy::RefList.end() )
	{
		Enemy* pEnemy = (*it);
		SceneObjectAnimated* pObj = pEnemy->getSceneObjectAnimated();
		if(pObj)
		{
			newPos = pEnemy->getSceneObjectAnimated()->GetPosition();
			distance = aiEnemy->calculDistance( posPlayer, newPos );
			newPos = Vector3f(0,0,0);
			newAngle = 0;

			if ( distance <= attackRange )
			{
				if (pEnemy->getLife() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
				{
					aiEnemy->enemyAIAttack( posPlayer, pEnemy->getSceneObjectAnimated()->GetPosition(), newAngle );
					pEnemy->changeState(Perso::ATTACK);
				}
				else
				{
					aiEnemy->enemyAIEvade( posPlayer );
					pEnemy->changeState(Perso::RUN);
				}
			}
			else if ( distance <= fieldOfView )
			{
				if (pEnemy->getLife() >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)
				{
					aiEnemy->enemyAIMoveTo( posPlayer, pEnemy->getSceneObjectAnimated()->GetPosition(), newPos, newAngle);
					pEnemy->changeState(Perso::RUN);
				}
				else
				{
					aiEnemy->enemyAIEvade( posPlayer );
					pEnemy->changeState(Perso::RUN);
				}
			}
			else
			{
				timeEnemy = pEnemy->timeSinceLastPath;
				aiEnemy->enemyAIPatrol( posPlayer, newPos, newAngle, timeEnemy);
				pEnemy->changeState(Perso::RUN);
				pEnemy->timeSinceLastPath = timeEnemy;
			}

			//(*it)->getSceneObjectAnimated()->SetTranslation(newPos.x, 0, newPos.z);
			//(*it)->getSceneObjectAnimated()->SetRotation(0, (float)newAngle, 0);

			Vector3f oldPos = pEnemy->getSceneObjectAnimated()->GetPosition();
			pEnemy->getSceneObjectAnimated()->SetPosition( oldPos.x+newPos.x, 0, oldPos.z+newPos.z );
			pEnemy->getSceneObjectAnimated()->SetRotation( 0, newAngle, 0 );

			NxActor* a = pEnemy->getArme()->getActor();
			if( a )
				a->clearBodyFlag( NX_BF_FROZEN_ROT_Y );

			pEnemy->getArme()->SetRotation( 0.f, -newAngle, 0.f );
			
			if( a )
				a->raiseBodyFlag( NX_BF_FROZEN_ROT_Y );
			a=NULL;

			it++;
			pEnemy->update( );
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
		while ((int)Enemy::RefList.size() < nbEnemy)
		{
			// Position aléatoire par rapport a la AIMap
			pair<int,int> posSpawn = aiEnemy->getPtrAStar()->randomSpawn();

			// Transpose les coordonnées de la AI map en coordonnées de jeu
			float spawnX = floor(float((posSpawn.first*scaleCurrMap)/precCurrMap)-scaleCurrMap/2);
			float spawnZ = floor(float((posSpawn.second*scaleCurrMap)/precCurrMap)-scaleCurrMap/2);
		
			Enemy* enemy = NULL;
  			/*if ( random( 0, 1 ))		enemy = new Alien( Vector3f(spawnX, 8.f, spawnZ) );
   			else				*/enemy = new MmeGrise( Vector3f(spawnX, 8.f, spawnZ) );
			
			enemy->Init();
			enemy->getSceneObjectAnimated()->SetControledCharacter(3.f, 7.f, enemy );

			physX::Link( pHero->getArme(), enemy->getSceneObjectAnimated() );
			physX::Link( enemy->getArme(), pHero->getSceneObjectAnimated() );
		}
	}
}
