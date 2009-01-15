#include "Crc32.h"

#include <string.h>
#include "CCRC32.h"

//******************************************************************

/***********************************************************
 * Donne le crc32 de la chaîne.
 * @param[in]	s : chaîne de caractères
 * @return	crc32 de la chaîne
 **********************************************************/
crc32 CRC32_GetCrc32( const char *s )
{
	CCRC32	crc32Computer;
	crc32	crc32Result;
	crc32Computer.Initialize();
	crc32Computer.FullCRC( (unsigned char*)s, (unsigned int)strlen( s ), &crc32Result );
	return crc32Result;
}
