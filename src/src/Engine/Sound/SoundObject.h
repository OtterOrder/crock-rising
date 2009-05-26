#ifndef		_SOUNDOBJECT_H_
#define		_SOUNDOBJECT_H_

//******************************************************************

#include	<string>
#include	<list>
#include	<al.h>

#include	"Core/Types/Vector.h"

//******************************************************************

class SoundObject
{
public:

	// Liste de r�f�rences sur tous les SoundObject
	static std::list< SoundObject* > RefList;

	//-- Enum publiques

	enum Flag
	{
		PAUSED_BY_SYSTEM	= 1<<0	// La pause a �t� d�clench�e par le SoundSystem
	};

	//-- Structures publiques
	
	struct Properties
	{
		float		gain;			// 0->1
		float		pitch;			// 0.1->..
		bool		looping;		// Bouclant
		bool		relative;		// Position relative au listener
		Vector3f	position;		// Position
		Vector3f	velocity;		// Vecteur vitesse

		// Propri�t�s en lecture seule
		bool		spatializable;	// Spatialisable en 3D (= son mono)
	};

	//-- M�thodes publiques
	
	SoundObject( const std::string &soundName );
	virtual ~SoundObject();

	void Play();
	void Pause( u32 flag = 0 );
	void Stop();
	bool IsPlaying() const;
	
	void SetGain( float gain );
	void SetPitch( float pitch );
	void SetLooping( bool looping );
	void SetRelative( bool relative );
	void SetPosition( const Vector3f &position );
	void SetVelocity( const Vector3f &velocity );
	//void SetSound( const std::string &soundName );

	// Donne toutes les propri�t�s de l'objet
	inline const Properties& GetProperties() const { return m_Properties; }
	inline const std::string& GetSound() const { return m_SoundName; }

protected:

	friend class SoundSystem;

	ALuint			m_SourceID;			// ID de la source openAL
	std::string		m_SoundName;		// Nom de la ressource son
	Properties		m_Properties;		// Propri�t�s de l'objet
	u32				m_Flags;			// Flags

	//-- M�thodes prot�g�es

	bool Init();						// Initialise l'objet
	void Release();						// D�truit l'objet
	void SetPropertiesFromAPI();		// R�cup�re les propri�t�s de l'objet � partir d'openAL

	void SetFlag( u32 flag );			// Active le flag
	void UnsetFlag( u32 flag );			// D�sactive le flag
	bool IsFlagSet( u32 flag ) const;	// V�rifie si le flag est activ�

};

//******************************************************************
#endif		//_SOUNDOBJECT_H_
