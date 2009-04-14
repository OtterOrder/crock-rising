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
	// Cr�ation du device
	if( !m_pDevice )
	{
		DirectSoundCreate8(
			&DSDEVID_DefaultPlayback,
			&m_pDevice,
			NULL
		);
	}

	// Cr�ation du listener
	if( m_pDevice && !m_pListener )
	{
		DSBUFFERDESC		bufferDesc;
		IDirectSoundBuffer	*pBuffer;
		
		ZeroMemory( &bufferDesc, sizeof(DSBUFFERDESC) );
		bufferDesc.dwSize	= sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags	= DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
		
		// On est oblig� de cr�er un buffer
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
// V�rifie si le moteur son est initialis�.
//**********************************************************
bool SoundSystem::IsInitialized()
{
	return m_pDevice
		&& m_pListener;
}

//**********************************************************
// D�truit le moteur son.
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
// Intialise le listener aux param�tres par d�faut.
//**********************************************************
void SoundSystem::SetListenerToDefault()
{
	ListenerDesc defaultDesc;
	
	defaultDesc.position		= Vector3f( 0.f, 0.f, 0.f );
	defaultDesc.velocity		= Vector3f( 0.f, 0.f, 0.f );
	defaultDesc.direction		= Vector3f( 0.f, 0.f, 1.f );
	defaultDesc.up				= Vector3f( 0.f, 1.f, 0.f );
	defaultDesc.distance		= 100.f;
	defaultDesc.rolloffFactor	= 0.1f;
	defaultDesc.dopplerFactor	= 0.3f;
	
	SetListenerDesc( defaultDesc );
}

//**********************************************************
// Change tous les param�tres du listener.
// @param[in]	desc : structure de param�tres
//**********************************************************
void SoundSystem::SetListenerDesc( const ListenerDesc &desc )
{
	m_ListenerDesc = desc;

	if( m_pListener )
	{
		m_pListener->SetAllParameters(
			(DS3DLISTENER*)&m_ListenerDesc,
			DS3D_DEFERRED // Mise � jour en diff�r�..
		);
	}
}

//**********************************************************
// Donne tous les param�tres du listener.
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
	DS3DLISTENER ds3dDesc;
	
	ds3dDesc.dwSize				= sizeof(DS3DLISTENER);
	ds3dDesc.vPosition			= position;
	ds3dDesc.vVelocity			= velocity;
	ds3dDesc.vOrientFront		= direction;
	ds3dDesc.vOrientTop			= up;
	ds3dDesc.flDistanceFactor	= distance;
	ds3dDesc.flRolloffFactor	= rolloffFactor;
	ds3dDesc.flDopplerFactor	= dopplerFactor;
	
	return ds3dDesc;
}
