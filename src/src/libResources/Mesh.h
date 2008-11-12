#ifndef		_Mesh_H
#define		_Mesh_H

//******************************************************************

#include	"Resource.h"

#include	<TinyXml/tinyxml.h>

//******************************************************************

class Mesh : public Resource
{
	public:
		
		Mesh(void);
		Mesh(TCHAR strTexName, LPDIRECT3DDEVICE9 pDevice);
		~Mesh();

		ResourceResult			SetDevice	( LPDIRECT3DDEVICE9 pDevice ); //Modifie le device grace au renderer
		LPDIRECT3DTEXTURE9		GetTex		(){ return m_pTex; }

	private:

		LPDIRECT3DDEVICE9		m_pDevice;		//Device utilisé par le renderer
		LPDIRECT3DTEXTURE9		m_pTex;			//Pointeur vers la texture

};

//******************************************************************
#endif		// _Mesh_H
