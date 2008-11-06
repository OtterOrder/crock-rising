#include "TextureManager.h"
#include "Texture.h"

//******************************************************************

TextureResult TextureManager::Clear( void )
{
	Texture *pTexture = NULL;
	
	while( !m_TextureList.empty() )
	{
		pTexture = m_TextureList.front();
		delete pTexture;
		m_TextureList.pop_front();
	}
	return TEX_SUCCEED;
}

/*TextureResult TextureManager::Load(crc32 texture)
{
	return TEX_FAILED;
}*/