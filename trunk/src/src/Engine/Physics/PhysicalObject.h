#pragma once

#include	<list>
#include	"NxPhysics.h"
#include	"Resources/Mesh.h"


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
