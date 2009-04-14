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
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< SoundSystem >;

public:

	// Param�tres de listener
	struct ListenerDesc
	{
		Vector3f	position;		// Position
		Vector3f	velocity;		// Vitesse de d�placement
		Vector3f	direction;		// Direction
		Vector3f	up;				// Up
		float		maxDistance;	// Distance max de propagation du son (en m�tres)
		float		rolloffFactor;	// Facteur d'att�nuation en fonction de la distance
		float		dopplerFactor;	// Facteur d'effet Doppler

		operator DS3DLISTENER();	// Op�rateur de cast
	};

	// G�n�ral
	void Initialize();		// Initialise le moteur son
	bool IsInitialized();	// V�rifie si le moteur son est initialis�
	void Release();			// D�truit le moteur son

	// Param�tres du listener
	void SetListenerToDefault();
	void SetListenerPosition( const Vector3f &position );
	void SetListenerVelocity( const Vector3f &velocity );
	void SetListenerOrientation( const Vector3f &direction, const Vector3f &up );
	ListenerDesc GetListenerDesc() const;

	//..

protected:

	//-- Donn�es prot�g�es

	IDirectSound8			*m_pDevice;			// Device audio
	IDirectSound3DListener8	*m_pListener;		// "Ecouteur"
	ListenerDesc			m_ListenerDesc;		// Param�tres de l'�couteur

	//-- M�thodes prot�g�es

	SoundSystem();
	~SoundSystem();

};

//******************************************************************
#endif		//_SoundSystem_H_
