#include "Resource.h"

//******************************************************************

Resource::Resource( void )
{
	m_Crc32	= CRC32_NULL;
	m_Flags	= 0;
}


crc32 Resource::GetCrc32( void ) const
{
	return m_Crc32;
}
