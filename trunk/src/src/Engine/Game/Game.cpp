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
	m_PrevLevelID	= CRC32_NULL;
	m_ObjList		= &Object::RefList;
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
	ObjIt	obj, lastObj;
	
	// Update des objets 3D
	
	obj = m_ObjList->begin();
	lastObj = m_ObjList->end();

	while( obj != lastObj )
	{
		(*obj)->Update();
		++obj;
	}

	// Update des objets 2D
	Object2D::UpdateAll();
	
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
	m_PrevLevelID = GetLevelID();
	
	if( m_CurrentLevel )
		delete m_CurrentLevel;

	// Ici il faudra peut-être gérer un temps de déchargement/chargement..

	m_CurrentLevel = Level::NewLevel( levelID );
	m_CurrentLevel->Init();
}

/***********************************************************
 * Donne l'ID du niveau courant.
 * @return	l'ID du niveau, ou CRC32_NULL si aucun
 **********************************************************/
crc32 Game::GetLevelID( void ) const
{
	if( m_CurrentLevel )
		return m_CurrentLevel->GetLevelID();

	return CRC32_NULL;
}

/***********************************************************
 * Donne l'ID du niveau précédent.
 * @return	l'ID du niveau précédent, CRC32_NULL si aucun
 **********************************************************/
crc32 Game::GetPrevLevelID( void ) const
{
	return m_PrevLevelID;
}
