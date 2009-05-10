#include	"Sound.h"

#include	"Vorbis/vorbisfile.h"

//******************************************************************

//**********************************************************
// Constructeur.
//**********************************************************
Sound::Sound()
: m_BufferID(AL_NONE), m_BufferSize(0),
m_Frequency(0), m_Channels(0), m_Bits(0)
{
}

//**********************************************************
// Destructeur.
//**********************************************************
Sound::~Sound()
{
	if( m_BufferID != AL_NONE )
	{
		alDeleteBuffers( 1, &m_BufferID );
		m_BufferID = AL_NONE;
	}
}

//**********************************************************
// Charge le son dans un buffer openAL.
// @param[in]	resource	: nom de la ressource
// @param[in]	param		: encodage du fichier (osef)
//**********************************************************
ResourceResult Sound::Load( std::string resource, ResourceParam param )
{
	// Création du buffer..
	alGenBuffers( 1, &m_BufferID );
	if( alGetError() != AL_NO_ERROR )
	{
		m_BufferID = AL_NONE;
		return RES_FAILED;
	}

	// Chargement des données sonores..
	LoadFromOgg( "../../data/sound/"+resource );

	// Récupérations des propriétés des données..
	alGetBufferi( m_BufferID, AL_SIZE, (ALint*)&m_BufferSize );
	alGetBufferi( m_BufferID, AL_FREQUENCY, (ALint*)&m_Frequency );
	alGetBufferi( m_BufferID, AL_CHANNELS, (ALint*)&m_Channels );
	alGetBufferi( m_BufferID, AL_BITS, (ALint*)&m_Bits );
	
	return RES_SUCCEED;
}

//**********************************************************
// Charge le son à partir d'un fichier Ogg.
// @param[in]	path : chemin vers le fichier
//**********************************************************
ResourceResult Sound::LoadFromOgg( std::string path )
{
	FILE *pFile;
	vorbis_info *pInfo;
	OggVorbis_File ovFile;
	ALsizei /*size,*/ freq;
	ALenum format;
	//ALvoid *data;
	//int bitStream;

	pFile = fopen( path.c_str(), "rb" );
	ov_open( pFile, &ovFile, NULL, 0 );

	pInfo	= ov_info( &ovFile, -1 );
	format	= ( pInfo->channels == 1 ) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	freq	= pInfo->rate;

	// Lecture des données..
	//ov_read( &ovFile, (char*)data, 0, 0, 2, 1, &bitStream );

	ov_clear( &ovFile );
	fclose( pFile );
	
	// Copie des données dans le buffer..
	//alBufferData( m_BufferID, format, data, size, freq );
	
	return RES_SUCCEED;
}
