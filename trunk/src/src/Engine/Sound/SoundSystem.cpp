#include	"SoundSystem.h"

//******************************************************************

//**********************************************************
// Constructeur.
//**********************************************************
SoundSystem::SoundSystem()
{
	m_pDevice	= NULL;
	m_pContext	= NULL;

	if( !Init() )
	{
		// Baaaaah ça marche pas..
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
// @return	vrai si l'initialisation a réussi
//**********************************************************
bool SoundSystem::Init()
{
	if( !m_pDevice )
	{
		// Ouverture du device audio par défaut..
		if( !( m_pDevice = alcOpenDevice( NULL ) ) )
			return false;
	}

	if( !m_pContext )
	{
		// Création et activation d'un contexte par défaut..
		m_pContext = alcCreateContext( m_pDevice, NULL );
		alcMakeContextCurrent( m_pContext );

		if( ALC_NO_ERROR != alcGetError( m_pDevice ) )
			return false;
	}

	// Propriétés du listener par défaut..
	SetListenerDescFromAPI();

	return true;
}

//**********************************************************
// Détruit le moteur son.
//**********************************************************
void SoundSystem::Release()
{
	// Il ne doit plus exister aucun buffer..
	
	// Désactivation et destruction du contexte
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

//**********************************************************
// Change le gain du listener = master gain.
// @param[in]	gain	: 0->1
//**********************************************************
void SoundSystem::SetListenerGain( float gain )
{
	m_ListenerDesc.gain = gain;
	alListenerf( AL_GAIN, (ALfloat)gain );
}

//**********************************************************
// Change la position du listener.
// @param[in]	position : position (dans le système de
//				coordonnées de DirectX)
//**********************************************************
void SoundSystem::SetListenerPosition( const Vector3f &position )
{
	m_ListenerDesc.position = position;
	alListener3f(
		AL_POSITION,
		(ALfloat)position.x,
		(ALfloat)position.y,
		(ALfloat)(-position.z)	// Passage en système OpenAL (=OpenGL)
	);
}

//**********************************************************
// Change la vitesse du listener.
// @param[in]	velocity	: vecteur vitesse
//**********************************************************
void SoundSystem::SetListenerVelocity( const Vector3f &velocity )
{
	m_ListenerDesc.velocity = velocity;
	alListener3f(
		AL_VELOCITY,
		(ALfloat)velocity.x,
		(ALfloat)velocity.y,
		(ALfloat)(-velocity.z)	// Passage en système OpenAL (=OpenGL)
	);
}

//**********************************************************
// Change l'orientation du listener.
// @param[in]	direction	: direction
// @param[in]	up			: vecteur up
//**********************************************************
void SoundSystem::SetListenerOrientation( const Vector3f &direction, const Vector3f &up )
{
	float values[6];

	m_ListenerDesc.direction	= direction;
	values[0]					= direction.x;
	values[1]					= direction.y;
	values[2]					= -direction.z;

	m_ListenerDesc.up			= up;
	values[3]					= up.x;
	values[4]					= up.y;
	values[5]					= -up.z;

	alListenerfv( AL_ORIENTATION, (ALfloat*)values );
}

//**********************************************************
// Récupère les propriétés du listener à partir d'openAL.
//**********************************************************
void SoundSystem::SetListenerDescFromAPI()
{
	float values[6];
	
	// Gain
	alGetListenerf( AL_GAIN, (ALfloat*)&m_ListenerDesc.gain );

	// Position
	alGetListenerfv( AL_POSITION, (ALfloat*)values );
	m_ListenerDesc.position.x = values[0];
	m_ListenerDesc.position.y = values[1];
	m_ListenerDesc.position.z = -values[2]; // Passage en système DirectX

	// Vitesse
	alGetListenerfv( AL_VELOCITY, values );
	m_ListenerDesc.velocity.x = values[0];
	m_ListenerDesc.velocity.y = values[1];
	m_ListenerDesc.velocity.z = -values[2];

	// Orientation (direction et up)
	alGetListenerfv( AL_ORIENTATION, values );
	m_ListenerDesc.direction.x = values[0];
	m_ListenerDesc.direction.y = values[1];
	m_ListenerDesc.direction.z = -values[2];
	m_ListenerDesc.up.x = values[3];
	m_ListenerDesc.up.y = values[4];
	m_ListenerDesc.up.z = -values[5];
}
