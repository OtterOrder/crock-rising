#ifndef _crockAI_H_
#define _crockAI_H_

#include	"../CrockRising/Characters/Enemy.h"
#include	"AIEnemy.h"
#include	<iostream>
#include	<math.h>
#include	<vector>

using namespace std;


class AIManager
{
public:
	// Mode disponible de l'AI, attaque/fuite, attaque seul, fuite seul
	enum typeAI	{	AI_NORMAL, AI_ONLY_ATTACK, AI_ONLY_EVADE	};

public:
	AIManager( bool spawn, int comportementAI, int fovEnemy, int rangeAttack, int scaleMap, int precise );
	~AIManager();

	void	update( Vector3f posPlayer, float elapsedTime, vector<Enemy*> listEnemy );

protected:
	void	spawn( int nbSpawn = -1 );

protected:
	static const int NB_GROUP_MAX = 1;

	Enemy*		listAIEnemy;
	Vector3f	newPos;
	int			newAngle;

	bool spawnInfini;
	int	nbGroup;
	int	typeAI;
	int fieldOfView;
	int attackRange;
	int	distance;
	int	hpsGroup;
	int	cptGeneral;

	AIEnemy* aiEnemy;
};

#endif //_crockAI_H_