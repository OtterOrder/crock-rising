#include "AStar.h"

AStar::AStar()
{
	Bitmap loadBmp;
	bool loadOk = loadBmp.loadBMP("../../data/mapAI/levelSphynx.bmp");

	if (loadOk)		loadBmp.convertImgToMatrix( tabChemin );
	else			std::cout << "Erreur chargement AIMap" << std::endl;

	listSpawn = loadBmp.getRandomSpawn();

	srand( (unsigned)time(NULL) );
}

AStar::~AStar(void)
{
}

pair<int,int> AStar::findWay( int debutX, int debutY, int finX, int finY )
{
	listOpen.clear();
	blackList.clear();
	lastWayPoint.first = -1;
	lastWayPoint.second = -1;
	maxIteration = 0;

	// Spécifie le point de départ et d'arrivée
	pair<int,int> noeudCourant;
	pointStart.parent.first		= debutX;
	pointStart.parent.second	= debutY;
	noeudCourant.first			= debutX;
	noeudCourant.second			= debutY;
	pointEnd.x = finX;
	pointEnd.y = finY;

	listOpen[noeudCourant] = pointStart;
	addToBlackList( noeudCourant );
	addSquareAdjacent( noeudCourant );

	// Tant qu'on est pas a la destination et qu'il est possible de trouvé un chemin
	while( !((noeudCourant.first == pointEnd.x) && (noeudCourant.second == pointEnd.y)) && (!listOpen.empty()) )
	{
        // on cherche le meilleur node de la liste ouverte, on sait qu'elle n'est pas vide donc il existe
        noeudCourant = bestNode( listOpen );

        // on le passe dans la liste fermee, il ne peut pas déjà y être
        addToBlackList(noeudCourant);
        addSquareAdjacent(noeudCourant);

		maxIteration++;

		// evite de faire ramer l'AI si les AI ne trouve pas de chemin
		if (maxIteration > 20)	break;
    }

	// Si le node courant est au meme coordonnes que la destination, on a trouvé un chemin
    if ((noeudCourant.first == pointEnd.x) && (noeudCourant.second == pointEnd.y))
	{
        findCompleteWay();
    }

	return lastWayPoint;
}


// Calcul de la distance euclidienne entre 2 points
// Version float plus précis mais moins rapide avec la fonction distance 
// qui retourne un float avec la racine carré de la distance euclidienne
// return sqrt((float)distanceEuc);
int AStar::distance(int x1, int y1, int x2, int y2)
{
	int distanceEuc = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	return distanceEuc;
}

// Retour true si un node est déja présent dans une liste
bool AStar::isInList(pair<int,int> n, listNode& l)
{
	listNode::iterator i = l.find(n);
    if (i == l.end())	return false;
    else				return true;
}

// Recupere les node et les ajoutes ou non a la liste ouverte
void AStar::addSquareAdjacent(pair <int,int>& n)
{
	node tmp;

    // Pour chaque case adjacent
    for (int i = n.first-1; i <= n.first+1; i++)
	{
        if ((i < 0) || (i > MAX_TAB-1))					continue;	// En dehors du tableau

		// Pour chaque case adjacent
        for (int j=n.second-1; j<=n.second+1; j++)
		{
            if ((j < 0) || (j > MAX_TAB-1))				continue;	// En dehors du tableau
            if ((i == n.first) && ( j== n.second))		continue;	// case actuelle n
            if (tabChemin[i][j] == 1)					continue;	// obstacle, terrain non franchissable

            pair<int,int> it(i,j);

			// Si le node n'est pas déjà présent dans la liste fermée
            if (!isInList(it, blackList))
			{
				tmp.costG = blackList[n].costG + distance(i, j, n.first, n.second);
				tmp.costH = (float)distance(i, j, pointEnd.x, pointEnd.y);
				tmp.costF = tmp.costG + tmp.costH;
                tmp.parent = n;

				// Si le node est déjà présent dans la liste ouverte, il faut comparer les couts
                if (isInList(it, listOpen))
				{
					// Si le nouveau chemin est meilleur, on update
					if (tmp.costF < listOpen[it].costF)		listOpen[it] = tmp;

                    // le node courant a un moins bon chemin, on ne change rien
                }
				else
				{
                    // Le node n'est pas présent dans la liste ouverte, on l'ajoute
                    listOpen[pair<int,int>(i,j)] = tmp;
                }
            }
        }
    }
}

// Retourne le meilleur node de la liste ouverte (celui qui a le cout le plus faible)
pair<int,int> AStar::bestNode(listNode& l)
{
    float m_coutf = l.begin()->second.costF;
    pair<int,int> m_noeud = l.begin()->first;

    for (listNode::iterator i = l.begin(); i!=l.end(); i++)
	{
        if (i->second.costF< m_coutf)
		{
            m_coutf = i->second.costF;
            m_noeud = i->first;
        }
	}

    return m_noeud;
}


// Passe un node de la liste ouverte a la liste fermée
void AStar::addToBlackList( pair<int,int>& p )
{
    node& n = listOpen[p];
    blackList[p] = n;

    // Suppression de la liste ouverte, ce n'est plus une solution explorable
	if (listOpen.erase(p)==0)	cerr << "Erreur, impossible de supprimer le node de la liste ouverte" << endl;
}


// Retrouve le chemin quand la destination est atteinte
void AStar::findCompleteWay()
{
	// Vide le chemin si il a déjà été rempli
	if ( !chemin.empty() )	
	{
		cptChemin = 0;
		chemin.clear();
	}

    // L'arrivée est le dernier élément de la liste fermée
    node& tmp = blackList[std::pair<int, int>(pointEnd.x,pointEnd.y)];

    struct point n;
    pair<int,int> prec;
    n.x = pointEnd.x;
    n.y = pointEnd.y;
    prec.first  = tmp.parent.first;
    prec.second = tmp.parent.second;
    //chemin.push_front(n);					// AVEC LIST
	chemin.push_back(n);

    while (prec != pair<int,int>(pointStart.parent.first,pointStart.parent.second))
	{
		cptChemin++;

		tabChemin[prec.first][prec.second] = 2;

        n.x = prec.first;
        n.y = prec.second;
        //chemin.push_front(n);					// AVEC LIST
		chemin.push_back(n);

        tmp = blackList[tmp.parent];
        prec.first  = tmp.parent.first;
        prec.second = tmp.parent.second;
    }

	if (cptChemin > 0)
	{
		lastWayPoint.first = chemin[cptChemin-1].x;
		lastWayPoint.second = chemin[cptChemin-1].y;
	}
}



pair<int,int> AStar::randomSpawn()
{
	int nbRand = (rand()%listSpawn.size());
	pair<int,int> xySpawn;

	xySpawn.first = listSpawn[nbRand].first;
	xySpawn.second = listSpawn[nbRand].second;

	return xySpawn;
}