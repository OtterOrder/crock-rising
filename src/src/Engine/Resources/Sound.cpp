#include	"Sound.h"

//#include	"Ogg/ogg.h"

//******************************************************************

//**********************************************************
// Constructeur.
//**********************************************************
Sound::Sound()
: m_BufferId(AL_NONE), m_BufferSize(0),
m_Frequency(0), m_Channels(0), m_Bits(0)
{
}

//**********************************************************
// Destructeur.
//**********************************************************
Sound::~Sound()
{
	if( m_BufferId != AL_NONE )
	{
		alDeleteBuffers( 1, &m_BufferId );
		m_BufferId = AL_NONE;
	}
}

//**********************************************************
// Charge le son dans un buffer openAL.
// @param[in]	resource	: nom de la ressource
// @param[in]	param		: encodage du fichier
//**********************************************************
ResourceResult Sound::Load( std::string resource, ResourceParam param )
{
	// Création du buffer..
	alGenBuffers( 1, &m_BufferId );
	if( alGetError() != AL_NO_ERROR )
	{
		m_BufferId = AL_NONE;
		return RES_FAILED;
	}

	// Chargement des données sonores..
	switch( (Sound::Encoding)(int)param )
	{
		/*case WAVE:
			//TODO
			break;*/
		
		case OGG:
			//TODO
			break;
	}

	// Récupérations des propriétés des données..
	alGetBufferi( m_BufferId, AL_SIZE, (ALint*)&m_BufferSize );
	alGetBufferi( m_BufferId, AL_FREQUENCY, (ALint*)&m_Frequency );
	alGetBufferi( m_BufferId, AL_CHANNELS, (ALint*)&m_Channels );
	alGetBufferi( m_BufferId, AL_BITS, (ALint*)&m_Bits );
	
	return RES_SUCCEED;
}
