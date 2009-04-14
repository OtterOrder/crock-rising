#include	"SoundSystem.h"

#include	"Objects/Object.h"

//******************************************************************

//**********************************************************
// Constructeur.
//**********************************************************
SoundSystem::SoundSystem()
{
	m_pDevice	= NULL;
	m_pListener	= NULL;

	SetListenerToDefault();
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
//**********************************************************
void SoundSystem::Initialize()
{
	// Création du device
	if( !m_pDevice )
	{
		DirectSoundCreate8(
			&DSDEVID_DefaultPlayback,
			&m_pDevice,
			NULL
		);
	}

	// Création du listener
	if( m_pDevice && !m_pListener )
	{
		DSBUFFERDESC		bufferDesc;
		IDirectSoundBuffer	*pBuffer;
		
		ZeroMemory( &bufferDesc, sizeof(DSBUFFERDESC) );
		bufferDesc.dwSize	= sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags	= DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
		
		// On est obligé de créer un buffer
		// standard pour en extraire le listener..
		m_pDevice->CreateSoundBuffer( &bufferDesc, &pBuffer, NULL );
		pBuffer->QueryInterface(
			IID_IDirectSound3DListener8,
			(LPVOID*)&m_pListener
		);
		pBuffer->Release();
	}
}

//**********************************************************
// Vérifie si le moteur son est initialisé.
//**********************************************************
bool SoundSystem::IsInitialized()
{
	return m_pDevice
		&& m_pListener;
}

//**********************************************************
// Détruit le moteur son.
//**********************************************************
void SoundSystem::Release()
{
	if( m_pDevice )
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	if( m_pListener )
	{
		m_pListener->Release();
		m_pListener = NULL;
	}
}

//**********************************************************
// Intialise le listener aux paramètres par défaut.
//**********************************************************
void SoundSystem::SetListenerToDefault()
{
	m_ListenerDesc.position			= Vector3f( 0.f, 0.f, 0.f );
	m_ListenerDesc.velocity			= Vector3f( 0.f, 0.f, 0.f );
	m_ListenerDesc.direction		= Vector3f( 0.f, 0.f, 1.f );
	m_ListenerDesc.up				= Vector3f( 0.f, 1.f, 0.f );
	m_ListenerDesc.maxDistance		= 100.f;
	m_ListenerDesc.rolloffFactor	= 0.1f;
	m_ListenerDesc.dopplerFactor	= 0.3f;
}

//**********************************************************
// Change la position du listener.
// @param[in]	position : position dans l'espace
//**********************************************************
void SoundSystem::SetListenerPosition( const Vector3f &position )
{
	m_ListenerDesc.position = position;
}

//**********************************************************
// Change la vitesse de déplacement du listener.
// @param[in]	velocity : vitesse
//**********************************************************
void SoundSystem::SetListenerVelocity( const Vector3f &velocity )
{
	m_ListenerDesc.velocity = velocity;
}

//**********************************************************
// Change l'orientation du listener.
// @param[in]	direction : vecteur direction
// @param[in]	up : vecteur up
//**********************************************************
void SoundSystem::SetListenerOrientation( const Vector3f &direction, const Vector3f &up )
{
	m_ListenerDesc.direction	= direction;
	m_ListenerDesc.up			= up;
}

//**********************************************************
// Donne tous les paramètres du listener.
//**********************************************************
SoundSystem::ListenerDesc SoundSystem::GetListenerDesc() const
{
	return m_ListenerDesc;
}

//**********************************************************
// Caste le ListenerDesc en DS3DLISTENER.
//**********************************************************
SoundSystem::ListenerDesc::operator DS3DLISTENER()
{
	DS3DLISTENER ds3dListener;
	ds3dListener.dwSize				= sizeof(DS3DLISTENER);
	ds3dListener.vPosition			= position;
	ds3dListener.vVelocity			= velocity;
	ds3dListener.vOrientFront		= direction;
	ds3dListener.vOrientTop			= up;
	ds3dListener.flDistanceFactor	= maxDistance;
	ds3dListener.flRolloffFactor	= rolloffFactor;
	ds3dListener.flDopplerFactor	= dopplerFactor;
	return ds3dListener;
}
