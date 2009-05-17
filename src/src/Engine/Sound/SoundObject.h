#ifndef		_SOUNDOBJECT_H_
#define		_SOUNDOBJECT_H_

//******************************************************************

#include	<string>
#include	<al.h>

#include	"Core/Types/Vector.h"

//******************************************************************

class SoundObject
{
public:

	struct Properties
	{
		float		gain;
		float		pitch;
		bool		looping;
		bool		relative;
		Vector3f	position;
	};
	
	SoundObject( const std::string &soundName );
	virtual ~SoundObject();

	void Play();
	void Pause();
	void Stop();
	bool IsPlaying() const;
	
	void SetGain( float gain );
	void SetPitch( float pitch );
	void SetLooping( bool looping );
	void SetRelative( bool relative );
	void SetPosition( const Vector3f &position );

	// Donne toutes les propri�t�s de l'objet
	inline const Properties& GetProperties() const { return m_Properties; }

protected:

	ALuint			m_SourceID;		// ID de la source openAL
	std::string		m_SoundName;	// Nom de la ressource son
	Properties		m_Properties;	// Propri�t�s de l'objet

	//-- M�thodes prot�g�es

	bool Init();					// Initialise l'objet
	void SetPropertiesFromAPI();	// R�cup�re les propri�t�s de l'objet � partir d'openAL

};

//******************************************************************
#endif		//_SOUNDOBJECT_H_
