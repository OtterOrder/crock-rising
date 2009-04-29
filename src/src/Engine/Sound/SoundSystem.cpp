#include	"SoundSystem.h"

//******************************************************************

//**********************************************************
// Constructeur.
//**********************************************************
SoundSystem::SoundSystem()
{
	m_pDevice	= NULL;
	m_pContext	= NULL;

	if( !Initialize() )
	{
		// Baaaaah �a marche pas..
	}
}

//**********************************************************
// Destructeur.
//**********************************************************
SoundSystem::~SoundSystem()
{
	Release();
}

//**********************************************************
// Initialise le moteur son.
// @return	vrai si l'initialisation a r�ussi
//**********************************************************
bool SoundSystem::Initialize()
{
	if( !m_pDevice )
	{
		// Ouverture du device audio par d�faut..
		if( !( m_pDevice = alcOpenDevice( NULL ) ) )
			return false;
	}

	if( !m_pContext )
	{
		// Cr�ation et activation d'un contexte par d�faut..
		m_pContext = alcCreateContext( m_pDevice, NULL );
		alcMakeContextCurrent( m_pContext );

		if( ALC_NO_ERROR != alcGetError( m_pDevice ) )
			return false;
	}
	return true;
}

//**********************************************************
// D�truit le moteur son.
//**********************************************************
void SoundSystem::Release()
{
	// Il ne doit plus exister aucun buffer..
	
	// D�sactivation et destruction du contexte
	alcMakeContextCurrent( NULL );
	alcDestroyContext( m_pContext );
	m_pContext = NULL;

	// Fermeture du device
	alcCloseDevice( m_pDevice );
	m_pDevice = NULL;
}

//**********************************************************
// Update.
//**********************************************************
/*void SoundSystem::Update()
{
	//TODO
}*/
