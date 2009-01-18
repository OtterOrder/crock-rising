#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include <d3dx9.h>
#include "../Core/Types/Vector.h"

//===========================================================================//
// Classe g�n�rique pour repr�senter un objet 3D                             //
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
	Vector3f GetPosition() {return m_Position;}

	//===========================================================================//
	// Fonction de transformation		    		                             //
	//===========================================================================//
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj) {}
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos) {}
	virtual void SetTransform(const D3DXMATRIX* world) {}

	//===========================================================================//
	// Fonction virtuelle d'affichage	    		                             //
	//===========================================================================//
	virtual void InitObject() {}
	virtual void InitDeviceData() {}
	virtual void Draw() {}
	virtual void FrameMove(float fElapsedTime) {}
	virtual void DeleteDeviceData() {}
	virtual void DeleteData() {}

protected:
	D3DXMATRIX			m_WorldMatrix;
	LPDIRECT3DDEVICE9	m_pDevice;
	Vector3f			m_Position;
};
