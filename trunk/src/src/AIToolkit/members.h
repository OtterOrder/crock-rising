#ifndef _members_H_
#define _members_H_

#include	<Characters/Enemy.h>
#include	<d3dx9.h>
#include	"AStar.h"
#include	"AIRess.h"
#include	"iostream"

class members
{
public:
	members( int idMembersAI, Vector3f pos );
	~members();

	void membersAttack( Vector3f posPlayer );
	void membersMoveTo( Vector3f posPlayer, float elapsedTime );
	void membersEvade();
	void membersPatrol();

	Vector3f position;
	Vector3f orientation;

	int idMembers;
	int stateMember;
	int hpRemaining;

	int posXScale;
	int posYScale;
	int posEXScale;
	int posEYScale;
	bool existPath;

	float timeBetFindPath;
	pair<int,int> nextWayPoint;

	Enemy*	getEnemy()	const	{	return m_pEnemy;	}

protected:
	Enemy*	m_pEnemy;
	AStar	aPath;
};

#endif