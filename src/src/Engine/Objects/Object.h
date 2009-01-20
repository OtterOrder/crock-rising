#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include <d3dx9.h>
#include "../Core/Types/Vector.h"

//===========================================================================//
// Classe générique pour représenter un objet 3D                             //
//===========================================================================//
class Object
{
public:
	//===========================================================================//
	// Constructeur									                             //
	//===========================================================================//
	Object( void );
	virtual ~Object() {}
	Object( float initPosX, float initPosY, float initPosZ );
	Object( D3DXVECTOR3 pos );

	//===========================================================================//
	// Fonction de transformation		    		                             //
	//===========================================================================//
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj) {}
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos) {}
	virtual void SetTransform(const D3DXMATRIX* world) {}

protected:
	D3DXMATRIX			m_WorldMatrix;
	LPDIRECT3DDEVICE9	m_pDevice;

};
