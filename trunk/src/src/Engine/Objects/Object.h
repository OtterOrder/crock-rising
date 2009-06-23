#ifndef		_Object_H
#define		_Object_H

//******************************************************************

#include	<list>
#include	<d3dx9.h>

#include	"../Core/Types/Vector.h"

//******************************************************************

// Classe générique pour représenter un objet 3D
class Object
{
public:
	
	// Liste de références sur les objets
	static std::list< Object* > RefList;
	
	// Constructeurs
	Object( void );
	Object( float initPosX, float initPosY, float initPosZ );
	Object( D3DXVECTOR3 pos );
	virtual ~Object( void );

	float m_vAngleX;
	float m_vAngleY;
	float m_vAngleZ;

	// Update
	virtual void Update(); // Méthode appelée chaque tour moteur

	// Fonctions de transformation
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj) {}
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos) {}
	virtual void SetTransform(const D3DXMATRIX* world) {}
	virtual void ApplyTransform(const D3DXMATRIX* world) {}

	Vector3f GetPosition();
	void SetTranslation( float x, float y, float z );
	void SetRotation( float angleX, float angleY, float angleZ );
	
	D3DXMATRIX* GetWorldMatrix ()	{ return &m_WorldMatrix; };
	void		SetWorldMatrix (D3DXMATRIX _worldmatrix)   {m_WorldMatrix=_worldmatrix;}
protected:
	
	D3DXMATRIX			m_WorldMatrix;
	LPDIRECT3DDEVICE9	m_pDevice;

private:

	void CommonInit( void ); // Initialisation commune à tous les constructeurs

};

//******************************************************************
#endif		//_Object_H
