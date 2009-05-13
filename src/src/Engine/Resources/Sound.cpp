#include	"Sound.h"

#include	<vector>
#include	<assert.h>
#include	"Vorbis/vorbisfile.h"

using namespace std;

//******************************************************************

#define		OGG_BUFFER_SIZE			32768		// 32kb
#define		OGG_BITS				16			// Nombre de bits par sample (8 ou 16)
#define		OGG_ENDIAN				0			// 0: Little-Endian // 1: Big-Endian
#define		OGG_SIGNED				1			// 0: unsigned // 1: signed

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
ResourceResult Sound::Load( string resource, ResourceParam param )
{
	// Création du buffer openAL..
	alGenBuffers( 1, &m_BufferID );
	if( alGetError() != AL_NO_ERROR )
	{
		m_BufferID = AL_NONE;
		return RES_FAILED;
	}

	// Chargement des données sonores..
	// TEMP: seulement du ogg pour le moment !
	if( !LoadFromOgg( "../../data/sound/" + resource ) )
	{
		// D'oh !
		assert( false );
	}

	// Récupérations des propriétés des données..
	alGetBufferi( m_BufferID, AL_SIZE, (ALint*)&m_BufferSize );
	alGetBufferi( m_BufferID, AL_FREQUENCY, (ALint*)&m_Frequency );
	alGetBufferi( m_BufferID, AL_CHANNELS, (ALint*)&m_Channels );
	alGetBufferi( m_BufferID, AL_BITS, (ALint*)&m_Bits );
	
	return RES_SUCCEED;
}

//**********************************************************
// Charge le son à partir d'un fichier Wave.
// @param[in]	path : chemin vers le fichier
// @return	vrai si le chargement est réussit
//**********************************************************
bool Sound::LoadFromWave( std::string path )
{
	//TODO
	
	return false;
}

//**********************************************************
// Charge le son à partir d'un fichier Ogg.
// @param[in]	path : chemin vers le fichier
// @return	vrai si le chargement est réussit
//**********************************************************
bool Sound::LoadFromOgg( string path )
{
	FILE			*pFile;
	vorbis_info		*pInfo;
	OggVorbis_File	oggFile;
	vector<char>	data;
	char			buffer[OGG_BUFFER_SIZE];
	int				bitStream;	// Ne sert à rien
	long			bytes;
	ALenum			format;
	ALsizei			freq;

	pFile = fopen( path.c_str(), "rb" );
	ov_open( pFile, &oggFile, NULL, 0 );

	// On récupère les infos sur le son
	pInfo	= ov_info( &oggFile, -1 );
	format	= ( pInfo->channels == 1 ) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	freq	= pInfo->rate;

	do
	{
		// On lit les données du fichier par bloc de 32kb..
		bytes = ov_read( &oggFile, buffer, OGG_BUFFER_SIZE, OGG_ENDIAN, OGG_BITS>>3, OGG_SIGNED, &bitStream );
		data.insert( data.end(), buffer, buffer + bytes );
	}
	while( bytes > 0 ); // bytes = nombre d'octets lus

	ov_clear( &oggFile );
	fclose( pFile );

	// On copie les données dans le buffer openAL..
	alBufferData( m_BufferID, format, (ALvoid*)data[0], (ALsizei)data.size(), freq );

	return true;
}
