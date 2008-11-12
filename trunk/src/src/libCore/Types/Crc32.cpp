#include "Crc32.h"

#include <string.h>
#include "CCRC32/CCRC32.h"

//******************************************************************

/***********************************************************
 * Donne le crc32 de la cha�ne.
 * @param[in]	s : cha�ne de caract�res
 * @return	crc32 de la cha�ne
 **********************************************************/
crc32 GetCrc32( const char *s )
{
	CCRC32	crc32Computer;
	crc32	crc32Result;
	crc32Computer.Initialize();
	crc32Computer.FullCRC( (unsigned char*)s, strlen( s ), &crc32Result );
	return crc32Result;
}
