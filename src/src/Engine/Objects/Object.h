#ifndef		_Object_H
#define		_Object_H

//******************************************************************

#include	<list>
#include	<d3dx9.h>

#include	"../Core/Types/Vector.h"

//******************************************************************

// Classe g�n�rique pour repr�senter un objet 3D
class Object
{
public:
	
	// Liste de r�f�rences sur les objets
	static std::list< Object* > RefList;
	
	// Constructeurs
	Object( void );
	Object( float initPosX, float initPosY, float initPosZ );
	Object( D3DXVECTOR3 pos );
	virtual ~Object( void );

	// Update
	virtual void Update(); // M�thode appel�e chaque tour moteur

	// Fonctions de transformation
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj) {}
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos) {}
	virtual void SetTransform(const D3DXMATRIX* world) {}

	D3DXMATRIX* GetTransform() { return &m_WorldMatrix; } //pour recup�rer sa position

protected:

	D3DXMATRIX			m_WorldMatrix;
	LPDIRECT3DDEVICE9	m_pDevice;

private:

	void CommonInit( void ); // Initialisation commune � tous les constructeurs

};

//******************************************************************
#endif		//_Object_H