#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	<list>
#include	<d3dx9.h>

#include	"../Core/Types/Vector.h"

//===========================================================================//
// Classe g�n�rique pour repr�senter un objet 3D                             //
//===========================================================================//
class Object
{
public:
	
	//static std::list< Object* > RefList; // Liste de r�f�rences sur les objets
	
	//===========================================================================//
	// Constructeur									                             //
	//===========================================================================//
	Object			( void );
	Object			( float initPosX, float initPosY, float initPosZ );
	Object			( D3DXVECTOR3 pos );
	virtual ~Object	( void );

	//===========================================================================//
	// Fonction de transformation		    		                             //
	//===========================================================================//
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj) {}
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos) {}
	virtual void SetTransform(const D3DXMATRIX* world) {}

protected:

	D3DXMATRIX			m_WorldMatrix;
	LPDIRECT3DDEVICE9	m_pDevice;

private:

	// M�thodes d'ajout/retrait de l'objet de la liste de r�f�rences
	void	RegisterRef	( void );
	void	ReleaseRef	( void );

};
