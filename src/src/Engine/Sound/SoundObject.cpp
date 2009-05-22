#include	"SoundObject.h"

#include	"Resources/ResourceManager.h"
#include	"Resources/Sound.h"

//******************************************************************

//**********************************************************
// Constructeur avec paramètre.
// @param[in]	soundName : nom de la ressource son
//**********************************************************
SoundObject::SoundObject( const std::string &soundName )
: m_SourceID(AL_NONE), m_SoundName(soundName)
{
	if( !Init() )
	{
		// D'oh!
	}
}

//**********************************************************
// Destructeur.
//**********************************************************
SoundObject::~SoundObject()
{
	Release();
}

//**********************************************************
// Joue le son, ou le relance s'il était en pause. Relance
// le son du début s'il était déjà en train de jouer.
//**********************************************************
void SoundObject::Play()
{
	alSourcePlay( m_SourceID );
}

//**********************************************************
// Pause le son.
//**********************************************************
void SoundObject::Pause()
{
	alSourcePause( m_SourceID );
}

//**********************************************************
// Stoppe le son.
//**********************************************************
void SoundObject::Stop()
{
	alSourceStop( m_SourceID );
}

//**********************************************************
// Vérifie si le son est en train de jouer.
// @return	vrai si le son joue
//**********************************************************
bool SoundObject::IsPlaying() const
{
	ALint state;
	alGetSourcei( m_SourceID, AL_SOURCE_STATE, &state );
	return state == AL_PLAYING;
}

//**********************************************************
// Change le son.
// @param[in]	soundName : nom de la ressource son
//**********************************************************
/*void SoundObject::SetSound( const std::string &soundName )
{
	Release();
	m_SoundName = soundName;
	if( !Init() )
	{
		// D'oh!
	}
}*/

//**********************************************************
// Change le gain du son.
// @param[in]	gain : 0->1
//**********************************************************
void SoundObject::SetGain( float gain )
{
	m_Properties.gain = gain;
	alSourcef( m_SourceID, AL_GAIN, (ALfloat)gain );
}

//**********************************************************
// Change le pitch du son.
// @param[in]	pitch : 0.1->..
//**********************************************************
void SoundObject::SetPitch( float pitch )
{
	m_Properties.pitch = pitch;
	alSourcef( m_SourceID, AL_PITCH, (ALfloat)pitch );
}

//**********************************************************
// Fait boucler ou non le son.
// @param[in]	looping : vrai ou faux
//**********************************************************
void SoundObject::SetLooping( bool looping )
{
	m_Properties.looping = looping;
	alSourcei( m_SourceID, AL_LOOPING, ((looping)?AL_TRUE:AL_FALSE) );
}

//**********************************************************
// Indique dans quel repère est valable la position.
// @param[in]	relative : vrai -> position relative au
//				listener, faux -> position absolue
//**********************************************************
void SoundObject::SetRelative( bool relative )
{
	m_Properties.relative = relative;
	alSourcei( m_SourceID, AL_SOURCE_RELATIVE, ((relative)?AL_TRUE:AL_FALSE) );
}

//**********************************************************
// Change la position du son.
// @param[in]	position : position (dans le système de
//				coordonnées de DirectX)
//**********************************************************
void SoundObject::SetPosition( const Vector3f &position )
{
	m_Properties.position = position;
	alSource3f(
		m_SourceID, AL_POSITION,
		(ALfloat)position.x,
		(ALfloat)position.y,
		(ALfloat)(-position.z)	// Passage en système OpenAL (=OpenGL)
	);
}

//**********************************************************
// Initialise l'objet.
// @return	vrai si l'initialisation est réussit
//**********************************************************
bool SoundObject::Init()
{
	// Création de la source openAL..
	alGenSources( 1, &m_SourceID );

	// On récupère les valeurs par défaut..
	SetPropertiesFromAPI();
	
	// On charge la ressource sonore et on associe son buffer à la source..
	Sound *sound = ResourceManager::GetInstance()->Load<Sound>( m_SoundName );
	alSourcei( m_SourceID, AL_BUFFER, sound->GetBufferID() );
	
	// On récupère les propriétés dépendantes de la ressource
	m_Properties.spatializable = ( sound->GetChannels() == 1 );

	return true;
}

//**********************************************************
// Détruit l'objet.
//**********************************************************
void SoundObject::Release()
{
	if( m_SourceID != AL_NONE )
	{
		Stop();
		alSourcei( m_SourceID, AL_BUFFER, AL_NONE ); // Il faut liberer le buffer avant de suppimer la source !
		alDeleteSources( 1, &m_SourceID );
		m_SourceID = AL_NONE;
	}

	// On supprime la ressource..
	ResourceManager::GetInstance()->Remove<Sound>( m_SoundName );
}

//**********************************************************
// Récupère les propriétés de l'objet à partir d'openAL.
//**********************************************************
void SoundObject::SetPropertiesFromAPI()
{
	alGetSourcef( m_SourceID, AL_GAIN, (ALfloat*)&m_Properties.gain );
	alGetSourcef( m_SourceID, AL_PITCH, (ALfloat*)&m_Properties.pitch );
	alGetSourcei( m_SourceID, AL_LOOPING, (ALint*)&m_Properties.looping );
	alGetSourcei( m_SourceID, AL_SOURCE_RELATIVE, (ALint*)&m_Properties.relative );
	
	// Position
	alGetSource3f(
		m_SourceID, AL_POSITION,
		(ALfloat*)&m_Properties.position.x,
		(ALfloat*)&m_Properties.position.y,
		(ALfloat*)&m_Properties.position.z
	);
	m_Properties.position.z = -m_Properties.position.z; // Passage en système DirectX
}
