#ifndef _ASTAR_H_
#define _ASTAR_H_

#include "AIMap.h"
#include <map>
#include <math.h>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

//-----------------------------------
struct node
{
	pair<int,int> parent;	// Son node parent...

	float costG;	// Cout pour aller du départ au node considéré
	float costH;	// Cout pour aller du node considéré à l'arrivé
	float costF;	// Somme des node précédents
};

struct point
{
    int x, y;
};

typedef map< pair<int, int>, node > listNode;

//-----------------------------------
class AStar
{
public:
	AStar();
	~AStar(void);

	pair<int,int> findWay( int debutX, int debutY, int finX, int finY );

	pair<int,int>	bestNode(listNode& l);				// Retourne le meilleur node de la liste ouverte
	int	distance( int x1, int y1, int x2, int y2 );		// Calcul de la distance euclidienne entre 2 points
	bool	isInList(pair<int,int> n, listNode& l);		// Retour true si un node est déja présent dans une liste
	void	addSquareAdjacent(pair <int,int>& n);		// Recupere les node et les ajoutes ou non a la liste ouverte	
	void	addToBlackList(pair<int,int>& p);			// Passe un node de la liste ouverte a la liste fermée
	void	findCompleteWay();							// Retrouve le chemin quand la destination est atteinte

	point pointEnd;
	node pointStart;

	listNode listOpen;
	listNode blackList;

	vector<point> chemin;
	int cptChemin;

	static const int MAX_TAB = 256;
	int tabChemin[MAX_TAB][MAX_TAB];

	pair<int,int> lastWayPoint;
};


#endif
