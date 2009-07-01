#ifndef _crockAI_H_
#define _crockAI_H_

#include	"../CrockRising/Characters/Perso.h"   
#include	"../CrockRising/Characters/Enemy.h"
#include	"../CrockRising/Characters/Alien.h"
#include	"../CrockRising/Characters/MmeGrise.h"
#include	"AIEnemy.h"
#include	"AIMap.h"
#include	<iostream>
#include	<math.h>
#include	<vector>
#include	<stdlib.h>
#include	<time.h>
#include	<map>

using namespace std;


class AIManager
{
public:
	// Mode disponible de l'AI, attaque/fuite, attaque seul, fuite seul
	enum typeAI	{	AI_NORMAL, AI_ONLY_ATTACK, AI_ONLY_EVADE	};

public:
	AIManager( bool spawn, int comportementAI, int nbMaxEnemy, int fovEnemy, int rangeAttack, int scaleMap, int precise );
	~AIManager();

	void	update( Vector3f posPlayer, float elapsedTime, vector<Enemy*> listEnemy );
	void	updateSpawn();


protected:
	static const int NB_GROUP_MAX = 1;

	Enemy*		listAIEnemy;
	Vector3f	newPos;
	int			newAngle;

	bool spawnInfini;
	int nbEnemy;
	int	nbGroup;
	int	typeAI;
	int fieldOfView;
	int attackRange;
	int	distance;
	int	hpsGroup;
	int scaleCurrMap;
	int precCurrMap;

	AIEnemy*	aiEnemy;
	Bitmap		mapSpawn;
};

#endif //_crockAI_H_