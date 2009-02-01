#pragma once

#include	<list>
#include	"NxPhysics.h"
#include	"Resources/Mesh.h"
#include	"Physicalizer.h"

enum ShapeType
{
	BOX,
	SPHERE,
	CAPSULE
};

//////////////////////////////////////////////////////////////////////////
//Interface des classes de description des Shape. la classe 
//ShapeDescritption contient les paramêtres communs. Non exhaustif
//////////////////////////////////////////////////////////////////////////

struct ShapeDescription
{
	ShapeDescription(float aDensity = 1.0f, float aMass = 1.0f, Vector3f aglobalPosition = Vector3f(0.0f, 0.0f, 0.0f))
		: density( aDensity ), mass( aMass ), globalPosition( aglobalPosition )
	{
	}

	float    density;		//[0, inf[ def : 0
	float    mass;			//[0, inf[ def : 0
	Vector3f globalPosition;
};

//Les structures qui suivent sont particulières à chaque shapes
struct BoxDescription : public ShapeDescription
{
	BoxDescription(Vector3f aDimension = Vector3f(0.0f, 0.0f, 0.0f), float aDensity = 1.0f, float aMass = 1.0f, Vector3f aglobalPosition = Vector3f(0.0f, 0.0f, 0.0f)) 
		: ShapeDescription( aDensity, aMass, aglobalPosition), dimension( aDimension)
	{ }

	Vector3f dimension;
};

struct SphereDescription : public ShapeDescription
{
	SphereDescription(float aRadius = 1.0f, float aDensity = 1.0f, float aMass = 1.0f, Vector3f aglobalPosition = Vector3f(0.0f, 0.0f, 0.0f)) 
		: ShapeDescription( aDensity, aMass, aglobalPosition), radius( aRadius)
	{ }
	float radius;
};

struct CapsuleDescription : public ShapeDescription
{
	CapsuleDescription(float aRadius = 1.0f, float aHeight = 1.0f, float aDensity = 1.0f, float aMass = 1.0f, Vector3f aglobalPosition = Vector3f(0.0f, 0.0f, 0.0f)) 
		: ShapeDescription( aDensity, aMass, aglobalPosition), radius( aRadius ), height( aHeight)
	{ }
	float radius, height;
};


////////////////////////////////////////////////////////////////////////////
// Structure material comprenant toutes les valeurs utiles de NxMaterials //
////////////////////////////////////////////////////////////////////////////
struct Material
{
	Material(float aRestitution = 0.5,
		     float aStaticFriction = 0.5,
			 float aDynamicFriction = 0.5)
	{
		Restitution = aRestitution;
		StaticFriction = aStaticFriction;
		DynamicFriction = aDynamicFriction;
	}
	~Material(){}

	float Restitution;
	float StaticFriction;
	float DynamicFriction;
};

///////////////////////////////////////////////////////////////////////////
// Structure BoundingBox qui englobera les objets. Elles seront fournies //
// au moteur physique													 //
///////////////////////////////////////////////////////////////////////////
class BoundingBox
{
	bool			m_bDebugMode; //Si vrai, les bounding box se dessinent pour debugger.
	Material*		m_pMat; 
	int				m_empListe;
	//What else? Groupe de collision?

public:
	BoundingBox(ShapeDescription *ShapeDesc = &ShapeDescription(), ShapeType Type = BOX, Material* aMat = &Material());
	~BoundingBox(){}

	bool IsInCollision(const BoundingBox &po);
	void EnableDebugMode(bool enable){ m_bDebugMode = enable; }

	int getEmpListe(){ return m_empListe; }
};

