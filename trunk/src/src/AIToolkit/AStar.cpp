#include "AStar.h"

AStar::AStar()
{
	Bitmap loadBmp;
	bool loadOk = loadBmp.loadBMP("../../data/mapAI/levelTestAI.bmp");

	if (loadOk)		loadBmp.convertImgToMatrix( tabChemin );
	else			std::cout << "Erreur chargement AIMap" << std::endl;
}

AStar::~AStar(void)
{
}

pair<int,int> AStar::findWay( int debutX, int debutY, int finX, int finY )
{
	listeOuverte.clear();
	listeFermee.clear();
	lastWayPoint.first = -1;
	lastWayPoint.second = -1;

	// Spécifie le point de départ et d'arrivée
	pair<int,int> noeudCourant;
	depart.parent.first		= debutX;
	depart.parent.second	= debutY;
	noeudCourant.first		= debutX;
	noeudCourant.second		= debutY;
	arrivee.x = finX;
	arrivee.y = finY;

	listeOuverte[noeudCourant] = depart;
	ajouterListeFermee( noeudCourant );
	ajouterCasesAdjacentes( noeudCourant );

	// Tant qu'on est pas a la destination et qu'il est possible de trouvé un chemin
	while( !((noeudCourant.first == arrivee.x) && (noeudCourant.second == arrivee.y)) && (!listeOuverte.empty()) )
	{
        // on cherche le meilleur noeud de la liste ouverte, on sait qu'elle n'est pas vide donc il existe
        noeudCourant = meilleurNoeud( listeOuverte );

        // on le passe dans la liste fermee, il ne peut pas déjà y être
        ajouterListeFermee(noeudCourant);
        ajouterCasesAdjacentes(noeudCourant);
    }

	// Si le noeud courant est au meme coordonnes que la destination, on a trouver un chemin
    if ((noeudCourant.first == arrivee.x) && (noeudCourant.second == arrivee.y))
	{
        retrouverChemin();
    }

	return lastWayPoint;
}


// Calcul de la distance euclidienne entre 2 points
float AStar::distance(int x1, int y1, int x2, int y2)
{
	int distanceEuc = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	return sqrt((float)distanceEuc);
}

// Retour true si un noeud est déja présent dans une liste
bool AStar::presentDansListe(pair<int,int> n, listeNoeud& l)
{
	listeNoeud::iterator i = l.find(n);
    if (i == l.end())	return false;
    else				return true;
}

// Recupere les noeud et les ajoutes ou non a la liste ouverte
void AStar::ajouterCasesAdjacentes(pair <int,int>& n)
{
	noeud tmp;

    // Pour chaque case adjacent
    for (int i = n.first-1; i <= n.first+1; i++)
	{
        if ((i < 0) || (i > MAX_TAB-1))					continue;	// En dehors du tableau

		// Pour chaque case adjacent
        for (int j=n.second-1; j<=n.second+1; j++)
		{
            if ((j < 0) || (j > MAX_TAB-1))				continue;	// En dehors du tableau
            if ((i == n.first) && ( j== n.second))		continue;	// case actuelle n
            if (tabChemin[i][j] == 1)					continue;	// obstace, terrain non franchissable

            pair<int,int> it(i,j);

			// Si le noeud n'est pas déjà présent dans la liste fermée
            if (!presentDansListe(it, listeFermee))
			{
				tmp.coutG = listeFermee[n].coutG + distance(i, j, n.first, n.second);
				tmp.coutH = distance(i, j, arrivee.x, arrivee.y);
				tmp.coutF = tmp.coutG + tmp.coutH;
                tmp.parent = n;

				// Si le noeud est déjà présent dans la liste ouverte, il faut comparer les couts
                if (presentDansListe(it, listeOuverte))
				{
					// Si le nouveau chemin est meilleur, on update
					if (tmp.coutF < listeOuverte[it].coutF)		listeOuverte[it] = tmp;

                    // le noeud courant a un moins bon chemin, on ne change rien
                }
				else
				{
                    // Le noeud n'est pas présent dans la liste ouverte, on l'ajoute
                    listeOuverte[pair<int,int>(i,j)] = tmp;
                }
            }
        }
    }
}

// Retourne le meilleur noeud de la liste ouverte (celui qui a le cout le plus faible)
pair<int,int> AStar::meilleurNoeud(listeNoeud& l)
{
    float m_coutf = l.begin()->second.coutF;
    pair<int,int> m_noeud = l.begin()->first;

    for (listeNoeud::iterator i = l.begin(); i!=l.end(); i++)
	{
        if (i->second.coutF< m_coutf)
		{
            m_coutf = i->second.coutF;
            m_noeud = i->first;
        }
	}

    return m_noeud;
}


// Passe un noeud de la liste ouverte a la liste fermée
void AStar::ajouterListeFermee( pair<int,int>& p )
{
    noeud& n = listeOuverte[p];
    listeFermee[p] = n;

    // Suppression de la liste ouverte, ce n'est plus une solution explorable
	if (listeOuverte.erase(p)==0)	cerr << "Erreur, impossible de supprimer le noeud de la liste ouverte" << endl;
}


// Retrouve le chemin quand la destination est atteinte
void AStar::retrouverChemin()
{
	// Vide le chemin si il a déjà été rempli
	if ( !chemin.empty() )	
	{
		cptChemin = 0;
		chemin.clear();
	}

    // L'arrivée est le dernier élément de la liste fermée
    noeud& tmp = listeFermee[std::pair<int, int>(arrivee.x,arrivee.y)];

    struct point n;
    pair<int,int> prec;
    n.x = arrivee.x;
    n.y = arrivee.y;
    prec.first  = tmp.parent.first;
    prec.second = tmp.parent.second;
    //chemin.push_front(n);					// AVEC LIST
	chemin.push_back(n);

    while (prec != pair<int,int>(depart.parent.first,depart.parent.second))
	{
		cptChemin++;

		tabChemin[prec.first][prec.second] = 2;

        n.x = prec.first;
        n.y = prec.second;
        //chemin.push_front(n);					// AVEC LIST
		chemin.push_back(n);

        tmp = listeFermee[tmp.parent];
        prec.first  = tmp.parent.first;
        prec.second = tmp.parent.second;
    }

	if (cptChemin > 0)
	{
		lastWayPoint.first = chemin[cptChemin-1].x;
		lastWayPoint.second = chemin[cptChemin-1].y;
	}
/*
	for (int i=0; i<36; i++)
	{
		for (int j=0; j<36; j++)	
		{
			switch (tabChemin[i][j])
			{
			case 0:		cout << "." << " ";	break;
			case 1:		cout << "I" << " ";	break;
			case 2:		cout << "*" << " ";	break;
			}
		}
		cout << endl;	
	}
*/
}