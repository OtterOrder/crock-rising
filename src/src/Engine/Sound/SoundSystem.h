#ifndef		_SoundSystem_H_
#define		_SoundSystem_H_

//******************************************************************

#include	<dsound.h>
#include	"Core/Math.h"
#include	"Core/Singleton.h"

//******************************************************************

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
		Vector3f	velocity;		// Vitesse de d�placement (vecteur unitaire / seconde)
		Vector3f	direction;		// Direction
		Vector3f	up;				// Vecteur up
		float		distanceFactor;	// Longueur (en m�tres) d'un vecteur unitaire
		float		rolloffFactor;	// Facteur d'att�nuation en fonction de la distance (0->10)
		float		dopplerFactor;	// Facteur d'effet Doppler (0->10)

		operator DS3DLISTENER();	// Op�rateur de cast
	};

	// G�n�ral
	void Initialize();		// Initialise le moteur son
	void Update();			// Update
	void Release();			// D�truit le moteur son

	// Param�tres du listener
	void SetListenerToDefault();
	void SetListenerDesc( const ListenerDesc &desc );
	ListenerDesc GetListenerDesc() const;

	// D�place la listener
	void MoveListener( const Vector3f &position, const Vector3f &velocity,
		const Vector3f &direction, const Vector3f &up );

	// Musique

protected:

	//-- Donn�es prot�g�es

	IDirectSound8			*m_pDevice;			// Device audio
	IDirectSound3DListener8	*m_pListener;		// "Ecouteur"
	ListenerDesc			m_ListenerDesc;		// Param�tres de l'�couteur

	//-- M�thodes prot�g�es

	SoundSystem();
	~SoundSystem();

	// Donne la description � l'entit� DirectSound du listener
	inline void SetListenerParam( const ListenerDesc &desc )
	{
		if( m_pListener ){
			m_pListener->SetAllParameters(
				(DS3DLISTENER*)&desc,
				DS3D_DEFERRED // Mise � jour en diff�r�..
			);
		}
	}

};

//******************************************************************
#endif		//_SoundSystem_H_
