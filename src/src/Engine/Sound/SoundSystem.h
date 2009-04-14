#ifndef		_SoundSystem_H_
#define		_SoundSystem_H_

//******************************************************************

#include	<dsound.h>
#include	"Core/Math.h"
#include	"Core/Singleton.h"

//******************************************************************

class Object;

class SoundSystem : public Singleton< SoundSystem >
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< SoundSystem >;

public:

	// Paramètres de listener
	struct ListenerDesc
	{
		Vector3f	position;		// Position
		Vector3f	velocity;		// Vitesse de déplacement
		Vector3f	direction;		// Direction
		Vector3f	up;				// Up
		float		maxDistance;	// Distance max de propagation du son (en mètres)
		float		rolloffFactor;	// Facteur d'atténuation en fonction de la distance
		float		dopplerFactor;	// Facteur d'effet Doppler

		operator DS3DLISTENER();	// Opérateur de cast
	};

	// Général
	void Initialize();		// Initialise le moteur son
	bool IsInitialized();	// Vérifie si le moteur son est initialisé
	void Release();			// Détruit le moteur son

	// Paramètres du listener
	void SetListenerToDefault();
	void SetListenerPosition( const Vector3f &position );
	void SetListenerVelocity( const Vector3f &velocity );
	void SetListenerOrientation( const Vector3f &direction, const Vector3f &up );
	ListenerDesc GetListenerDesc() const;

	//..

protected:

	//-- Données protégées

	IDirectSound8			*m_pDevice;			// Device audio
	IDirectSound3DListener8	*m_pListener;		// "Ecouteur"
	ListenerDesc			m_ListenerDesc;		// Paramètres de l'écouteur

	//-- Méthodes protégées

	SoundSystem();
	~SoundSystem();

};

//******************************************************************
#endif		//_SoundSystem_H_
