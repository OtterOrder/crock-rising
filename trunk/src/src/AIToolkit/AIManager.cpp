#include "AIManager.h"


AIManager::AIManager( bool spawn, int comportementAI ) 
	: nbGroup(0), spawnInfini(spawn), typeAI(comportementAI), cptGeneral(0)
{
}

AIManager::~AIManager()
{
}

vector<AIEnemy*> AIManager::initEnnemy( int nbGroup, int nbMember )
{
	cptGeneral = 0;

	for ( int i=0; i<nbGroup; i++ )
	for ( int j=0; j<nbMember; j++ )
	{
		listEnemy.push_back( new AIEnemy(cptGeneral, i, j, 100, Vector3f(i*20.f,0.f,j*20.f), Vector3f(0.f,0.f,0.f)) );
		cptGeneral++;
	}

	return listEnemy;
}

void AIManager::update( Vector3f posPlayer, float elapsedTime, vector<AIEnemy*> listAIEnemy )
{
	/////////////////////////////////////////////
	// Met a jour les positions des ennemies avant de les déplacer
	listEnemy = listAIEnemy;

	/////////////////////////////////////////////
	// Gère le respawn
	if ( nbGroup < NB_GROUP_MAX && spawnInfini)
	{
		while ( nbGroup < NB_GROUP_MAX )
		{
			//groupEnnemyAI.push_back( new group( nbGroup) );
			nbGroup++;
		}
	}

	/////////////////////////////////////////////
	// Gère les états et transitions
	cptGeneral = 0;
	for ( vector<AIEnemy*>::iterator it = listAIEnemy.begin(); it != listAIEnemy.end(); )
	{
		// Calcul la distance entre le joueur et l'ennemi pour en déduire son état
		listAIEnemy[cptGeneral]->setElapsedTime( elapsedTime );
		distance = AIRess::calculDistance( posPlayer, listAIEnemy[cptGeneral]->positEnnemy );

		if ( distance <= DISTANCE_ATTACK )		//**** TODO **** Rajouter champ de vision ****
		{
			if (listAIEnemy[cptGeneral]->hpsEnemy >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)	
				listAIEnemy[cptGeneral]->enemyAIAttack();
			else											
				listAIEnemy[cptGeneral]->enemyAIEvade( posPlayer );
		}
		else if ( distance <= FIELD_OF_VIEW )
		{
			if (listAIEnemy[cptGeneral]->hpsEnemy >= 30 || AI_ONLY_ATTACK && !AI_ONLY_EVADE)	
				listAIEnemy[cptGeneral]->enemyAIMoveTo( posPlayer );
			else											
				listAIEnemy[cptGeneral]->enemyAIEvade( posPlayer );
		}
		else
		{
			listAIEnemy[cptGeneral]->enemyAIPatrol(posPlayer);
		}

		cptGeneral++;
		it++;
	}
}

void AIManager::spawn( int nbSpawn )
{
}