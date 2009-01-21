#include	"Game.h"

#include	"Level.h"

//******************************************************************

/***********************************************************
 * Constructeur.
 **********************************************************/
Game::Game( void )
{
	m_CurrentLevel = NULL;
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
 * Update du jeu, appell�e � chaque tour moteur.
 **********************************************************/
void Game::Update( void )
{
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

	// Ici il faudra peut-�tre g�rer un temps
	// de d�chargement/chargement..

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