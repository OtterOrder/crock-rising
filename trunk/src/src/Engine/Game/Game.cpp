#include	"Game.h"

#include	<stdlib.h>
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
 * Update du jeu, appellée à chaque tour moteur.
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

	// TODO: ici il faudra peut-être gérer un temps
	// de déchargement/chargement..

	m_CurrentLevel = Level::NewLevel( levelID );
}

/***********************************************************
 * Donne l'ID du niveau courant.
 * @return	l'ID du niveau courant, ou CRC32_NULL si aucun
 **********************************************************/
crc32 Game::GetCurrentLevel( void )
{
	if( m_CurrentLevel )
		return m_CurrentLevel->GetLevelID();
	
	return CRC32_NULL;
}
