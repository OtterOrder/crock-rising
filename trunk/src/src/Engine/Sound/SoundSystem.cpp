#include	"SoundSystem.h"

#include	<assert.h>
#include	<al.h>
#include	"SoundObject.h"

//******************************************************************

//**********************************************************
// Constructeur.
//**********************************************************
SoundSystem::SoundSystem()
{
	m_pDevice	= NULL;
	m_pContext	= NULL;
	m_pMusic	= NULL;

	// Propriétés de la musique par défaut..
	m_MusicProperties.gain	= 1.f;
	m_MusicProperties.pitch	= 1.f;

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
	SetListenerPropertiesFromAPI();

	return true;
}

//**********************************************************
// Détruit le moteur son.
//**********************************************************
void SoundSystem::Release()
{
	// Destruction de la musique
	ReleaseMusic();
	
	// Il ne doit plus exister aucune entité openAL !
	assert( SoundObject::RefList.empty() );
	
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
	m_ListenerProperties.gain = gain;
	alListenerf( AL_GAIN, (ALfloat)gain );
}

//**********************************************************
// Change la position du listener.
// @param[in]	position : position (dans le système de
//				coordonnées de DirectX)
//**********************************************************
void SoundSystem::SetListenerPosition( const Vector3f &position )
{
	m_ListenerProperties.position = position;
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
	m_ListenerProperties.velocity = velocity;
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

	m_ListenerProperties.direction	= direction;
	values[0]						= direction.x;
	values[1]						= direction.y;
	values[2]						= -direction.z;

	m_ListenerProperties.up			= up;
	values[3]						= up.x;
	values[4]						= up.y;
	values[5]						= -up.z;

	alListenerfv( AL_ORIENTATION, (ALfloat*)values );
}

//**********************************************************
// Récupère les propriétés du listener à partir d'openAL.
//**********************************************************
void SoundSystem::SetListenerPropertiesFromAPI()
{
	float values[6];
	
	// Gain
	alGetListenerf( AL_GAIN, (ALfloat*)&m_ListenerProperties.gain );

	// Position
	alGetListenerfv( AL_POSITION, (ALfloat*)values );
	m_ListenerProperties.position.x = values[0];
	m_ListenerProperties.position.y = values[1];
	m_ListenerProperties.position.z = -values[2]; // Passage en système DirectX

	// Vitesse
	alGetListenerfv( AL_VELOCITY, values );
	m_ListenerProperties.velocity.x = values[0];
	m_ListenerProperties.velocity.y = values[1];
	m_ListenerProperties.velocity.z = -values[2];

	// Orientation (direction et up)
	alGetListenerfv( AL_ORIENTATION, values );
	m_ListenerProperties.direction.x = values[0];
	m_ListenerProperties.direction.y = values[1];
	m_ListenerProperties.direction.z = -values[2];
	m_ListenerProperties.up.x = values[3];
	m_ListenerProperties.up.y = values[4];
	m_ListenerProperties.up.z = -values[5];
}

//**********************************************************
// Pause tous les sons en cours de lecture.
//**********************************************************
void SoundSystem::PauseAll()
{
	std::list<SoundObject*>::iterator it = SoundObject::RefList.begin();
	while( it != SoundObject::RefList.end() )
	{
		if( (*it)->IsPlaying() )
		{
			// Flag : la pause est déclenchée par le SoundSystem
			(*it)->Pause( SoundObject::PAUSED_BY_SYSTEM );
		}
		++it;
	}
}

//**********************************************************
// Relance tous les sons dont la pause a été déclenchée
// par PauseAll.
//**********************************************************
void SoundSystem::ResumeAll()
{
	std::list<SoundObject*>::iterator it = SoundObject::RefList.begin();
	while( it != SoundObject::RefList.end() )
	{
		if( (*it)->IsFlagSet( SoundObject::PAUSED_BY_SYSTEM ) )
		{
			// On ne relance que les sons qui ont
			// été pausés par le SoundSystem
			(*it)->Play();
		}
		++it;
	}
}

//**********************************************************
// Stoppe tous les sons.
//**********************************************************
void SoundSystem::StopAll()
{
	std::list<SoundObject*>::iterator it = SoundObject::RefList.begin();
	while( it != SoundObject::RefList.end() )
	{
		(*it)->Stop();
		++it;
	}
}

//**********************************************************
// Joue la musique, ou la relance si elle était en pause.
// NE relance PAS la musique si elle était déjà en train
// de jouer !
//**********************************************************
void SoundSystem::PlayMusic()
{
	if( m_pMusic && !IsMusicPlaying() )
	{
		m_pMusic->Play();
	}
}

//**********************************************************
// Pause la musique.
//**********************************************************
void SoundSystem::PauseMusic()
{
	if( m_pMusic )
	{
		m_pMusic->Pause();
	}
}

//**********************************************************
// Stoppe la musique.
//**********************************************************
void SoundSystem::StopMusic()
{
	if( m_pMusic )
	{
		m_pMusic->Stop();
	}
}

//**********************************************************
// Vérifie si la musique est en train de jouer.
// @return	vrai si la musique joue
//**********************************************************
bool SoundSystem::IsMusicPlaying() const
{
	if( m_pMusic )
	{
		return m_pMusic->IsPlaying();
	}
	return false;
}

//**********************************************************
// Change la musique. La ressource sonore doit comporter
// au moins 2 canaux (stéréo), sinon elle est spatialisable
// et ça ne devrait pas être possible pour une musique.
// @param[in]	musicName : nom de la ressource son
//**********************************************************
void SoundSystem::SetMusic( const std::string &musicName )
{
	ReleaseMusic();
	m_pMusic = new SoundObject( musicName );

	if( m_pMusic->GetProperties().spatializable )
	{
		// La musique ne doit pas être spatialisable en
		// 3D, si c'est le cas, on laisse tomber..
		ReleaseMusic();
		return;
	}
	
	// On conserve les propriétés..
	m_pMusic->SetGain( m_MusicProperties.gain );
	m_pMusic->SetPitch( m_MusicProperties.pitch );
	m_pMusic->SetLooping( true ); // La musique est forcément bouclante
}

//**********************************************************
// Change le gain de la musique.
// @param[in]	gain : 0->1
//**********************************************************
void SoundSystem::SetMusicGain( float gain )
{
	m_MusicProperties.gain = gain;
	if( m_pMusic )
	{
		m_pMusic->SetGain( gain );
	}
}

//**********************************************************
// Change le pitch de la musique.
// @param[in]	pitch : 0.1->..
//**********************************************************
void SoundSystem::SetMusicPitch( float pitch )
{
	m_MusicProperties.pitch = pitch;
	if( m_pMusic )
	{
		m_pMusic->SetPitch( pitch );
	}
}

//**********************************************************
// Supprime la musique.
//**********************************************************
void SoundSystem::ReleaseMusic()
{
	if( m_pMusic )
	{
		delete m_pMusic;
		m_pMusic = NULL;
	}
}
