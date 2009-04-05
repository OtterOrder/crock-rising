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

	// Update
	virtual void Update(); // Méthode appelée chaque tour moteur

	// Fonctions de transformation
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj) {}
	virtual void SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos) {}
	virtual void SetTransform(const D3DXMATRIX* world) {}

	Vector3f GetPosition() { return m_vPosition; }
	void SetTranslation( float x, float y, float z );
	void SetRotation( int angleX, int angleY, int angleZ );
	
	D3DXMATRIX* GetWorldMatrix ()	{ return &m_WorldMatrix; };
protected:
	
	D3DXMATRIX			m_WorldMatrix;
	LPDIRECT3DDEVICE9	m_pDevice;

	Vector3f m_vPosition;
	int m_vAngleX, m_vAngleY, m_vAngleZ; //angle en degres

private:

	void CommonInit( void ); // Initialisation commune à tous les constructeurs

};

//******************************************************************
#endif		//_Object_H
