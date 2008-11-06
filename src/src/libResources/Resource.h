#ifndef		_Resource_H
#define		_Resource_H

//******************************************************************

#include	<libCore/Types/Crc32.h>

//******************************************************************

class Resource
{
	public:
		
		Resource			( void );			// Constructeur
		virtual ~Resource	( void ){}			// Destructeur

		crc32	GetCrc32	( void ) const;		// Donne le CRC32 de la ressource


	protected:
		
		crc32		m_Crc32;			// CRC32 de la ressource
};

//******************************************************************
#endif		// _Resource_H
