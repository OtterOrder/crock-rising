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
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< SoundSystem >;

public:

	// G�n�ral
	bool Initialize();		// Initialise le moteur son
	void Release();			// D�truit le moteur son
	//void Update();			// Update

	//TODO

protected:

	//-- Donn�es prot�g�es

	ALCdevice		*m_pDevice;		// Device audio
	ALCcontext		*m_pContext;	// Contexte

	//-- M�thodes prot�g�es

	SoundSystem();
	~SoundSystem();

};

//******************************************************************
#endif		//_SOUNDSYSTEM_H_
