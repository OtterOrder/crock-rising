#ifndef		_SOUNDSYSTEM_H_
#define		_SOUNDSYSTEM_H_

//******************************************************************

#include	<list>
#include	<alc.h>
#include	<al.h>

#include	"Core/Math.h"
#include	"Core/Singleton.h"

//******************************************************************

class SoundObject;

class SoundSystem : public Singleton< SoundSystem >
{
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< SoundSystem >;

public:

	struct ListenerDesc
	{
		float		gain;
		Vector3f	position;
		Vector3f	velocity;
		Vector3f	direction;
		Vector3f	up;
	};

	// G�n�ral
	bool Init();		// Initialise le moteur son
	void Release();		// D�truit le moteur son
	//void Update();		// Update

	// Listener
	void SetListenerGain( float gain );
	void SetListenerPosition( const Vector3f &position );
	void SetListenerVelocity( const Vector3f &velocity );
	void SetListenerOrientation( const Vector3f &direction, const Vector3f &up );

	// Donne toutes les propri�t�s du listener
	inline const ListenerDesc& GetListenerDesc() const { return m_ListenerDesc; }

protected:

	//-- Donn�es prot�g�es

	ALCdevice		*m_pDevice;			// Device audio
	ALCcontext		*m_pContext;		// Contexte
	ListenerDesc	m_ListenerDesc;		// Propri�t�s du listener

	//-- M�thodes prot�g�es

	SoundSystem();
	~SoundSystem();

	void SetListenerDescFromAPI();		// R�cup�re les propri�t�s du listener � partir d'openAL

};

//******************************************************************
#endif		//_SOUNDSYSTEM_H_
