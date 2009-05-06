#ifndef		_RESOURCE_SOUND_H_
#define		_RESOURCE_SOUND_H_

//******************************************************************

#include	<al.h>
#include	"Resource.h"

//******************************************************************

class Sound : public Resource
{
public:

	enum Encoding	// Encodage du fichier source
	{
		//WAVE,
		OGG,
	};
	
	Sound();
	virtual ~Sound();

	virtual ResourceResult Load( std::string resource, ResourceParam param );

	// Accesseurs
	//inline int GetBufferSize() const	{ return m_BufferSize; }
	inline int GetFrequency() const		{ return m_Frequency; }
	inline int GetChannels() const		{ return m_Channels; }
	inline int GetBits() const			{ return m_Bits; }

protected:

	ALuint	m_BufferId;		// Id du buffer openAL
	int		m_BufferSize;	// Taille du buffer (octets)
	int		m_Frequency;	// Fr�quence du sample (Hz)
	int		m_Channels;		// Nombre de canaux utilis�s
	int		m_Bits;			// Nombre de bits par sample (?)

};

//******************************************************************
#endif		//_RESOURCE_SOUND_H_