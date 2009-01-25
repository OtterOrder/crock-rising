#include	"Game.h"

#include	"Level.h"
#include	"Objects/Object.h"
#include	"Objects/Object2D/Object2D.h"

//using namespace std;

//******************************************************************

/***********************************************************
 * Constructeur.
 **********************************************************/
Game::Game( void )
{
	m_CurrentLevel	= NULL;
	m_ObjList		= &Object::RefList;
	m_Obj2DList		= &Object2D::RefList;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
Game::~Game( void )
{
	if( m_CurrentLevel )
		delete m_CurrentLevel;
}

/***********************************************************
 * Update du jeu, appellée à chaque tour moteur.
 **********************************************************/
void Game::Update( void )
{
	ObjIt	obj		= m_ObjList->begin();
	Obj2DIt	obj2d	= m_Obj2DList->begin();
	
	// Update des objets 3d
	while( obj != m_ObjList->end() )
	{
		(*obj)->Update();
		++obj;
	}

	// Update des objets 2d
	while( obj2d != m_Obj2DList->end() )
	{
		(*obj2d)->Update();
		++obj2d;
	}
	
	if( m_CurrentLevel )
	{
		// Update du level courant
		m_CurrentLevel->Update();
	}
}

/***********************************************************
 * Change de niveau.
 * @param[in]	levelID	: ID du prochain niveau
 **********************************************************/
void Game::ChangeLevel( crc32 levelID )
{
	if( m_CurrentLevel )
		delete m_CurrentLevel;

	// Ici il faudra peut-être gérer un temps
	// de déchargement/chargement..

	m_CurrentLevel = Level::NewLevel( levelID );
	m_CurrentLevel->Init();
}

/***********************************************************
 * Donne l'ID du niveau courant.
 * @return	l'ID du niveau, ou CRC32_NULL si aucun
 **********************************************************/
crc32 Game::GetLevelID( void )
{
	if( m_CurrentLevel )
		return m_CurrentLevel->GetLevelID();
	
	return CRC32_NULL;
}
