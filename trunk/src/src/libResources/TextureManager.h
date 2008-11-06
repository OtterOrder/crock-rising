#ifndef		_TextureManager_H
#define		_TextureManager_H

//******************************************************************

#include	"ResourceManager.h"
#include	"Texture.h"

//******************************************************************

class TextureManager : public ResourceManager
{
	public:	
		TextureResult			Clear( void );
		//TextureResult			Load(crc32 texture);

	protected:
		std::list< Texture* >		m_TextureList;		// Liste des ressources chargées

};

//******************************************************************
#endif		// _TextureManager_H
