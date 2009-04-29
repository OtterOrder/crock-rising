#ifndef		_SOUNDSYSTEM_H_
#define		_SOUNDSYSTEM_H_

//******************************************************************

#include	<alc.h>
//#include	<al.h>
#include	"Core/Math.h"
#include	"Core/Singleton.h"

//******************************************************************

class SoundSystem : public Singleton< SoundSystem >
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< SoundSystem >;

public:

	// Général
	bool Initialize();		// Initialise le moteur son
	void Release();			// Détruit le moteur son
	//void Update();			// Update

	//TODO

protected:

	//-- Données protégées

	ALCdevice		*m_pDevice;		// Device audio
	ALCcontext		*m_pContext;	// Contexte

	//-- Méthodes protégées

	SoundSystem();
	~SoundSystem();

};

//******************************************************************
#endif		//_SOUNDSYSTEM_H_
