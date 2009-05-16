#include	"Game.h"

#include	<assert.h>

#include	"Level.h"
#include	"Objects/Object.h"
#include	"Objects/2D/Object2D.h"

//using namespace std;

//******************************************************************

//**********************************************************
// Constructeur.
//**********************************************************
Game::Game( void )
{
	m_Backup		= NULL;
	m_CurrentLevel	= NULL;
	m_PrevLevelID	= CRC32_NULL;
	m_StartLevelID	= CRC32_NULL;
	m_ObjList		= &Object::RefList;
}

//**********************************************************
// Destructeur.
//**********************************************************
Game::~Game( void )
{
	if( m_Backup )
		delete m_Backup;
	
	if( m_CurrentLevel )
		delete m_CurrentLevel;
}

//**********************************************************
// Lance le niveau de départ (renseigné par SetStartLevel).
//**********************************************************
void Game::Start( void )
{
	ChangeLevel( m_StartLevelID );
}

//**********************************************************
// Update du jeu, appellée à chaque tour moteur.
//**********************************************************
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

//**********************************************************
// Enregistre la sauvegarde du jeu. On ne doit enregistrer
// cette structure qu'une seule fois au lancement du jeu..
// @param[in]	pBackup : pointeur sur la sauvegarde
//**********************************************************
void Game::StoreBackup( Backup *pBackup )
{
	assert( !m_Backup );
	m_Backup = pBackup;
}

//**********************************************************
// Donne la sauvegarde du jeu.
// @return	pointeur sur la sauvegarde du jeu
//**********************************************************
Game::Backup* Game::GetBackup( void ) const
{
	assert( m_Backup );
	return m_Backup;
}

//**********************************************************
// Positionne le niveau de départ. Cette méthode enregistre
// sur quel niveau le jeu devra démarrer une fois que tous
// les modules du moteur seront initialisés.
// @param[in]	levelID	: ID du niveau de départ
//**********************************************************
void Game::SetStartLevel( crc32 levelID )
{
	m_StartLevelID = levelID;
}

//**********************************************************
// Change de niveau. Attention, cette méthode appelle l'Init
// du niveau, tous les modules du moteur doivent avoir été
// initialisés avant. Pour le premier niveau, utilisez
// SetStartLevel.
// @param[in]	levelID	: ID du prochain niveau
//**********************************************************
void Game::ChangeLevel( crc32 levelID )
{
	m_PrevLevelID = GetLevelID();
	
	if( m_CurrentLevel )
	{
		delete m_CurrentLevel;
		m_CurrentLevel = NULL;
	}

	// Ici il faudra peut-être gérer un temps de déchargement/chargement..

	m_CurrentLevel = Level::NewLevel( levelID );
	m_CurrentLevel->Init();
}

//**********************************************************
// Donne l'ID du niveau courant.
// @return	l'ID du niveau, ou CRC32_NULL si aucun
//**********************************************************
crc32 Game::GetLevelID( void ) const
{
	if( m_CurrentLevel )
		return m_CurrentLevel->GetLevelID();

	return CRC32_NULL;
}

//**********************************************************
// Donne l'ID du niveau précédent.
// @return	l'ID du niveau précédent, CRC32_NULL si aucun
//**********************************************************
crc32 Game::GetPrevLevelID( void ) const
{
	return m_PrevLevelID;
}
