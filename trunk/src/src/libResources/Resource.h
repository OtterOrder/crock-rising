#ifndef		_Resource_H
#define		_Resource_H

//******************************************************************

#include	<libCore/Types/Crc32.h>
#include	<d3d9.h>
#include	<d3dx9tex.h>

//******************************************************************
// Defines pour les ressources

enum ResourceResult
{
	RES_SUCCEED,
	RES_FAILED,

};

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
