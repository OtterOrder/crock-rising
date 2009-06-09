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
struct noeud
{
	pair<int,int> parent;	// Son noeud parent...

	float coutG;	// Cout pour aller du départ au noeud considéré
	float coutH;	// Cout pour aller du noeud considéré à l'arrivé
	float coutF;	// Somme des noeud précédents
};

struct point
{
    int x, y;
};

typedef map< pair<int, int>, noeud > listeNoeud;

//-----------------------------------
class AStar
{
public:
	AStar();
	~AStar(void);

	pair<int,int> findWay( int debutX, int debutY, int finX, int finY );

	pair<int,int>	meilleurNoeud(listeNoeud& l);			// Retourne le meilleur noeud de la liste ouverte
	float	distance( int x1, int y1, int x2, int y2 );		// Calcul de la distance euclidienne entre 2 points
	bool	presentDansListe(pair<int,int> n, listeNoeud& l);	// Retour true si un noeud est déja présent dans une liste
	void	ajouterCasesAdjacentes(pair <int,int>& n);			// Recupere les noeud et les ajoutes ou non a la liste ouverte	
	void	ajouterListeFermee(pair<int,int>& p);				// Passe un noeud de la liste ouverte a la liste fermée
	void	retrouverChemin();									// Retrouve le chemin quand la destination est atteinte

	point arrivee;
	noeud depart;

	listeNoeud listeOuverte;
	listeNoeud listeFermee;

	vector<point> chemin;
	int cptChemin;

	static const int MAX_TAB = 36;
	int tabChemin[MAX_TAB][MAX_TAB];

	pair<int,int> lastWayPoint;
};


#endif
