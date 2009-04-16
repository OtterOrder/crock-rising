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

	Initialize();
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
// Update.
//**********************************************************
void SoundSystem::Update()
{
	if( m_pListener )
	{
		// Application de toutes les modifs
		m_pListener->CommitDeferredSettings();
	}
}

//**********************************************************
// Détruit le moteur son.
//**********************************************************
void SoundSystem::Release()
{
	if( m_pListener )
	{
		m_pListener->Release();
		m_pListener = NULL;
	}
	
	if( m_pDevice ) // A détruire en dernier !
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

//**********************************************************
// Intialise le listener aux paramètres par défaut.
//**********************************************************
void SoundSystem::SetListenerToDefault()
{
	ListenerDesc defaultDesc;
	
	defaultDesc.position		= Vector3f( 0.f, 0.f, 0.f );
	defaultDesc.velocity		= Vector3f( 0.f, 0.f, 0.f );
	defaultDesc.direction		= Vector3f( 0.f, 0.f, 1.f );
	defaultDesc.up				= Vector3f( 0.f, 1.f, 0.f );
	defaultDesc.distanceFactor	= 1.f;
	defaultDesc.rolloffFactor	= 3.f;
	defaultDesc.dopplerFactor	= 1.f;
	
	SetListenerDesc( defaultDesc );
}

//**********************************************************
// Change tous les paramètres du listener.
// @param[in]	desc : structure de paramètres
//**********************************************************
void SoundSystem::SetListenerDesc( const ListenerDesc &desc )
{
	m_ListenerDesc = desc;
	SetListenerParam( m_ListenerDesc );
}

//**********************************************************
// Donne tous les paramètres du listener.
//**********************************************************
SoundSystem::ListenerDesc SoundSystem::GetListenerDesc() const
{
	return m_ListenerDesc;
}

//**********************************************************
// Déplace la listener.  A utiliser typiquement lorsque
// le listener doit suivre un objet/caméra/etc.
// @param[in]	position : position
// @param[in]	velocity : vitesse
// @param[in]	direction : direction
// @param[in]	up : vecteur up
//**********************************************************
void SoundSystem::MoveListener( const Vector3f &position, const Vector3f &velocity,
							   const Vector3f &direction, const Vector3f &up )
{
	m_ListenerDesc.position		= position;
	m_ListenerDesc.velocity		= velocity;
	m_ListenerDesc.direction	= direction;
	m_ListenerDesc.up			= up;
	
	SetListenerParam( m_ListenerDesc );
}

//**********************************************************
// Caste le ListenerDesc en DS3DLISTENER.
//**********************************************************
SoundSystem::ListenerDesc::operator DS3DLISTENER()
{
	DS3DLISTENER ds3dDesc;
	
	ds3dDesc.dwSize				= sizeof(DS3DLISTENER);
	ds3dDesc.vPosition			= position;
	ds3dDesc.vVelocity			= velocity;
	ds3dDesc.vOrientFront		= direction;
	ds3dDesc.vOrientTop			= up;
	ds3dDesc.flDistanceFactor	= distanceFactor;
	ds3dDesc.flRolloffFactor	= rolloffFactor;
	ds3dDesc.flDopplerFactor	= dopplerFactor;
	
	return ds3dDesc;
}
