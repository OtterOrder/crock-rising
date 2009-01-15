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

	m_CurrentLevel = LEVEL_NewLevel( levelID );
}
