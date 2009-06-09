#include "group.h"


group::group( int idGroupAI )
{
	// Défini leurs ids et leurs états de base
	idGroup = idGroupAI;

	// Creer les nouveaux membres
	for (int i=0; i<NB_MEMBERS; i++)	
	{
		membersEnnemyAI.push_back( new members( idGroup*100+i, Vector3f(i*100,0,idGroup*100) ));
	}
}

group::~group()
{
}

void group::groupChooseAction( Vector3f posPlayer, float elapsedTime )
{
	for( int k=0; k<group::NB_MEMBERS; k++)
	{
		// Si il a assez d'hps pour combattre détermine si il est a portée
		if ( membersEnnemyAI[k]->hpRemaining >= 50 )
		{
			int dist = AIRess::calculDistance( posPlayer, membersEnnemyAI[k]->getEnemy()->getSceneObjectAnimated()->GetPosition() );
			if ( dist <= 4 )
			{
				membersEnnemyAI[k]->membersAttack( posPlayer );
			}
			else
			{
				membersEnnemyAI[k]->membersMoveTo( posPlayer, elapsedTime );
			}

		}
		// Sinon il s'enfui
		else
			membersEnnemyAI[k]->membersEvade();
	}
}

void group::groupAttack( Vector3f posPlayer, float elapsedTime )
{
	// Tout les membres attaquent même si ils n'ont plus de hp
	for( int k=0; k<group::NB_MEMBERS; k++)
	{
		membersEnnemyAI[k]->membersAttack( posPlayer );
	}
}


void group::groupEvade( float elapsedTime )
{
	// Tout les membres s'enfuient même si ils sont attaqués
	for( int k=0; k<group::NB_MEMBERS; k++)
	{
		membersEnnemyAI[k]->membersEvade();
	}
}


void group::groupPatrol( float elapsedTime )
{
	// Tout les membres patrouillent en attendant une actions
	for( int k=0; k<group::NB_MEMBERS; k++)
	{
		membersEnnemyAI[k]->membersPatrol();
	}
}