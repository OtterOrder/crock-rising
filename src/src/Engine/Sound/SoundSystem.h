#ifndef		_SOUNDSYSTEM_H_
#define		_SOUNDSYSTEM_H_

//******************************************************************

#include	<string>
#include	<alc.h>

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

	//-- Structures publiques
	
	struct ListenerProperties
	{
		float		gain;			// 0->1
		Vector3f	position;		// Position
		Vector3f	velocity;		// Vecteur vitesse
		Vector3f	direction;		// Vecteur direction
		Vector3f	up;				// Vecteur up
	};

	struct MusicProperties
	{
		float		gain;			// 0->1
		float		pitch;			// 0.1->..
	};

	//-- M�thodes publiques

	bool Init();		// Initialise le moteur son
	void Release();		// D�truit le moteur son
	//void Update();		// Update

	// Listener
	void SetListenerGain( float gain );
	void SetListenerPosition( const Vector3f &position );
	void SetListenerVelocity( const Vector3f &velocity );
	void SetListenerOrientation( const Vector3f &direction, const Vector3f &up );

	// Donne toutes les propri�t�s du listener
	inline const ListenerProperties& GetListenerProperties() const { return m_ListenerProperties; }

	// Global
	void PauseAll();
	void ResumeAll();
	void StopAll();

	// Musique
	void PlayMusic();
	void PauseMusic();
	void StopMusic();
	bool IsMusicPlaying() const;
	
	void SetMusic( const std::string &musicName );
	void SetMusicGain( float gain );
	void SetMusicPitch( float pitch );

	// Donne toutes les propri�t�s de la musique
	inline const MusicProperties& GetMusicProperties() const { return m_MusicProperties; }


protected:

	//typedef void (SoundSystem::*UpdateCallback)( void );

	//-- Donn�es prot�g�es

	ALCdevice			*m_pDevice;				// Device audio
	ALCcontext			*m_pContext;			// Contexte
	ListenerProperties	m_ListenerProperties;	// Propri�t�s du listener
	
	SoundObject			*m_pMusic;				// Musique
	MusicProperties		m_MusicProperties;		// Propri�t�s de la musique

	//-- M�thodes prot�g�es

	SoundSystem();
	~SoundSystem();

	void SetListenerPropertiesFromAPI();	// R�cup�re les propri�t�s du listener � partir d'openAL
	void ReleaseMusic();					// Supprime la musique

};

//******************************************************************
#endif		//_SOUNDSYSTEM_H_
