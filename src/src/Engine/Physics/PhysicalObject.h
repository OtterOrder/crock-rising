#pragma once

#include	<list>
#include	"NxPhysics.h"
#include	"Resources/Mesh.h"

////////////////////////////////////////////////////////////////////////////
// Structure material comprenant toutes les valeurs utiles de NxMaterials //
////////////////////////////////////////////////////////////////////////////
struct Material
{
	Material(NxReal aRestitution = 0.5,
		     NxReal aStaticFriction = 0.5,
			 NxReal aDynamicFriction = 0.5)
	{
		Restitution = aRestitution;
		StaticFriction = aStaticFriction;
		DynamicFriction = aDynamicFriction;
	}
	~Material(){}

	NxReal Restitution;
	NxReal StaticFriction;
	NxReal DynamicFriction;
};

///////////////////////////////////////////////////////////////////////////
// Structure BoundingBox qui englobera les objets. Elles seront fournies //
// au moteur physique													 //
///////////////////////////////////////////////////////////////////////////
struct BoundingBox
{
	BoundingBox(Mesh* aPrimitive = NULL, NxMaterial* aMat = NULL);
	~BoundingBox(){}

	Mesh*		pPrimitive;
	NxMaterial* pMat; 
	//What else? Groupe de collision?
};

// TODO = adapter la bounding box à la maniere dont sont stockées les meshs
class PhysicalObject
{
	BoundingBox m_BoundingBox;
	bool m_bDebugMode; //Si vrai, les bounding box se dessinent pour debugger.
public:
	PhysicalObject(void);
	~PhysicalObject(void);

	BoundingBox getBoundingBox(){ return m_BoundingBox; }
	bool IsInCollision(const PhysicalObject &po);
	void EnableDebugMode(bool enable);
};
