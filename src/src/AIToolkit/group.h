#ifndef _group_H_
#define _group_H_

#include "members.h"

class group
{
public:
	group( int idGroupAI );
	~group();

	// Action possible du groupe
	void groupChooseAction( Vector3f posPlayer, float elapsedTime );
	void groupAttack( Vector3f posPlayer, float elapsedTime );
	void groupEvade( float elapsedTime );
	void groupPatrol( float elapsedTime );

	// Constitution d'un groupe
	std::vector<members*> membersEnnemyAI;
	int idGroup;

	static const int NB_MEMBERS = 1;
};


#endif