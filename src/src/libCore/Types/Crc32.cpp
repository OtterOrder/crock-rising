#include "Crc32.h"

#include "CCRC32/CCRC32.h"

//******************************************************************

/***********************************************************
 * Donne le crc32 de la chaîne.
 * @param[in]	s : chaîne de caractères
 * @return	crc32 de la chaîne
 **********************************************************/
crc32 GetCrc32( const unsigned char *s )
{
	crc32	crc32Result;
	CCRC32	crc32Computer;
	
	crc32Computer.Initialize();
	crc32Computer.FullCRC( s, sizeof( s ), &crc32Result );

	return crc32Result;
}
