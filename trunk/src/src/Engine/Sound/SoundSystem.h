#ifndef		_SoundSystem_H_
#define		_SoundSystem_H_

//******************************************************************

#include	<dsound.h>
#include	"Core/Math.h"
#include	"Core/Singleton.h"

//******************************************************************

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
		Vector3f	velocity;		// Vitesse de déplacement (vecteur unitaire / seconde)
		Vector3f	direction;		// Direction
		Vector3f	up;				// Vecteur up
		float		distanceFactor;	// Longueur (en mètres) d'un vecteur unitaire
		float		rolloffFactor;	// Facteur d'atténuation en fonction de la distance (0->10)
		float		dopplerFactor;	// Facteur d'effet Doppler (0->10)

		operator DS3DLISTENER();	// Opérateur de cast
	};

	// Général
	void Initialize();		// Initialise le moteur son
	void Update();			// Update
	void Release();			// Détruit le moteur son

	// Paramètres du listener
	void SetListenerToDefault();
	void SetListenerDesc( const ListenerDesc &desc );
	ListenerDesc GetListenerDesc() const;

	// Déplace la listener
	void MoveListener( const Vector3f &position, const Vector3f &velocity,
		const Vector3f &direction, const Vector3f &up );

	// Musique

protected:

	//-- Données protégées

	IDirectSound8			*m_pDevice;			// Device audio
	IDirectSound3DListener8	*m_pListener;		// "Ecouteur"
	ListenerDesc			m_ListenerDesc;		// Paramètres de l'écouteur

	//-- Méthodes protégées

	SoundSystem();
	~SoundSystem();

	// Donne la description à l'entité DirectSound du listener
	inline void SetListenerParam( const ListenerDesc &desc )
	{
		if( m_pListener ){
			m_pListener->SetAllParameters(
				(DS3DLISTENER*)&desc,
				DS3D_DEFERRED // Mise à jour en différé..
			);
		}
	}

};

//******************************************************************
#endif		//_SoundSystem_H_
