#ifndef		_Crc32_H
#define		_Crc32_H

//******************************************************************

#define		CRC32_NULL			0x00000000		// 0
#define		CRC32_MAX			0xffffffff		// CRC32 maximum

//******************************************************************

// D�finition du type crc32
typedef unsigned long crc32;

//******************************************************************

crc32	CRC32_GetCrc32	( const char *s );			// Donne le crc32 de la cha�ne

//******************************************************************
#endif		// _Crc32_H