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
	// Indispensable car Singleton doit pouvoir accéder aux
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

	// Général
	bool Init();		// Initialise le moteur son
	void Release();		// Détruit le moteur son
	//void Update();		// Update

	// Listener
	void SetListenerGain( float gain );
	void SetListenerPosition( const Vector3f &position );
	void SetListenerVelocity( const Vector3f &velocity );
	void SetListenerOrientation( const Vector3f &direction, const Vector3f &up );

	// Donne toutes les propriétés du listener
	inline const ListenerDesc& GetListenerDesc() const { return m_ListenerDesc; }

protected:

	//-- Données protégées

	ALCdevice		*m_pDevice;			// Device audio
	ALCcontext		*m_pContext;		// Contexte
	ListenerDesc	m_ListenerDesc;		// Propriétés du listener

	//-- Méthodes protégées

	SoundSystem();
	~SoundSystem();

	void SetListenerDescFromAPI();		// Récupère les propriétés du listener à partir d'openAL

};

//******************************************************************
#endif		//_SOUNDSYSTEM_H_
