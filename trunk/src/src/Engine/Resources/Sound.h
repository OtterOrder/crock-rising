#ifndef		_RESOURCE_SOUND_H_
#define		_RESOURCE_SOUND_H_

//******************************************************************

#include	<al.h>
#include	"Resource.h"

//******************************************************************

class Sound : public Resource
{
public:

	/*enum Encoding	// Encodage du fichier source
	{
		WAVE,
		OGG,
	};*/
	
	Sound();
	virtual ~Sound();

	virtual ResourceResult Load( std::string resource, ResourceParam param );

	// Accesseurs
	inline ALuint GetBufferID() const	{ return m_BufferID; }
	//inline int GetBufferSize() const	{ return m_BufferSize; }
	inline int GetFrequency() const		{ return m_Frequency; }
	inline int GetChannels() const		{ return m_Channels; }
	inline int GetBits() const			{ return m_Bits; }

protected:

	ALuint	m_BufferID;		// ID du buffer openAL
	int		m_BufferSize;	// Taille du buffer (octets)
	int		m_Frequency;	// Fréquence du sample (Hz)
	int		m_Channels;		// Nombre de canaux utilisés
	int		m_Bits;			// Nombre de bits par sample (16 en principe)

	//-- Méthodes protégées

	bool LoadFromWave( std::string path );
	bool LoadFromOgg( std::string path );

};

//******************************************************************
#endif		//_RESOURCE_SOUND_H_
