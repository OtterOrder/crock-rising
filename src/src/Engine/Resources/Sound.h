#ifndef		_RESOURCE_SOUND_H_
#define		_RESOURCE_SOUND_H_

//******************************************************************

#include	<al.h>
#include	"Resource.h"

//******************************************************************

class Sound : public Resource
{
public:

	Sound();
	virtual ~Sound();

protected:

	ALuint	m_Buffer;

};

//******************************************************************
#endif		//_RESOURCE_SOUND_H_
