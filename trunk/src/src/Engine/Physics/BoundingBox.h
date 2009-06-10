#ifndef		_BOUNDINGBOX_H
#define		_BOUNDINGBOX_H

#include	<list>

#include	"NxPhysics.h"
#include	"NxBoxController.h"
#include	"NxCapsuleController.h"

#include	"Resources/Mesh.h"
#include	"Trigger/UserData.h"
#include	"BoundingBoxLoader.h"

struct ListOfBoundingBox
{
	std::vector<PhysicBody*>	List;
	Vector3f					WorldPos;
	//Rotate?
	std::vector<NxShapeDesc*>	ShapeRefList; //Ne servira qu'à la destruction.

	void SetTriggerFuncs( void (*OnEnterFunc)(), void (*OnStayFunc)(), void (*OnLeaveFunc)());
	void MajPivot(Mesh* pMesh);
	void ReleaseList();
};

enum PhysicalObjectType
{
	ACTOR,
	CONTROLLER,
	NOPHYSICAL
};

NxVec3 VecToNxVec(const Vector3f V);
void Normalize(Vector3f &V);
float Norme(Vector3f V);

//Classe mere contenant les variables communes à toutes les descriptions
struct ShapeDescription
{
	ShapeDescription(){}
	ShapeDescription(float aDensity, float aMass, Vector3f aPos)
		: m_density(aDensity), m_mass(aMass), m_pos(aPos)
	{ m_linearVelocity = m_ReglagePivot = Vector3f(0.f, 0.f, 0.f); }

	float m_density, m_mass;
	Vector3f m_pos;
	Vector3f m_linearVelocity;
	Vector3f m_ReglagePivot;
	ShapeType m_type;
};
////Les structures qui suivent sont particulières à chaque shapes
struct BoxDescription : public ShapeDescription
{
	BoxDescription(Vector3f aDimension = Vector3f(1.0f, 1.0f, 1.0f), float aDensity = 1.0f,
		float aMass = 1.0f, Vector3f aPos = Vector3f(0.0f, 0.0f, 0.0f)) 
		: ShapeDescription( aDensity, aMass, aPos), m_dimension( aDimension)
	{ m_type = BOX; }

	Vector3f m_dimension;
};

struct SphereDescription : public ShapeDescription
{
	SphereDescription(float aRadius = 1.0f, float aDensity = 1.0f, float aMass = 1.0f,
		Vector3f aPos = Vector3f(0.0f, 0.0f, 0.0f)) 
		: ShapeDescription( aDensity, aMass, aPos), m_radius( aRadius)
	{ m_type = SPHERE; }

	float m_radius;
};

struct CapsuleDescription : public ShapeDescription
{
	CapsuleDescription(float aRadius = 1.0f, float aHeight = 1.0f, float aDensity = 1.0f,
		float aMass = 1.0f, Vector3f aPos = Vector3f(0.0f, 10.0f, 0.0f)) 
		: ShapeDescription( aDensity, aMass, aPos), m_radius( aRadius ), m_height( aHeight)
	{ m_type = CAPSULE; }

	float m_radius, m_height;
};

struct TriggerDescription : public ShapeDescription
{
	TriggerDescription(Vector3f aDimension = Vector3f(1.0f, 1.0f, 1.0f), Vector3f aPos = Vector3f(0.0f, 0.0f, 0.0f),		
	void (*aOnEnterFunc)() = NULL, void (*aOnLeaveFunc)() = NULL, void (*aOnStayFunc)() = NULL ) 
		: ShapeDescription( 0, 0, aPos), m_dimension( aDimension)
	{ 
		m_type = TRIGGER;
		m_OnEnterFunc = aOnEnterFunc;
		m_OnLeaveFunc = aOnLeaveFunc;
		m_OnStayFunc = aOnStayFunc;
	}

	Vector3f m_dimension;
	void (*m_OnEnterFunc)(); 
	void (*m_OnLeaveFunc)(); 
	void (*m_OnStayFunc)(); 
};




/////////////////////////////////////////////////////////
// Structure BoundingBox qui englobera les objets.	   //
// Elles seront fournies au moteur physique.		   //
// DEPRECATED										   //
/////////////////////////////////////////////////////////
// class BoundingBox
// {
// 	//What else? Groupe de collision?
// 
// 	void CreateBox( NxActorDesc& ActorDesc, ListOfBoundingBox& List, PhysicBody* Pb, bool IsTrigger );
// 	void CreateSphere( NxActorDesc& ActorDesc, ListOfBoundingBox& List, PhysicBody* Pb, bool IsTrigger  );
// 	void CreateCapsule( NxActorDesc& ActorDesc, ListOfBoundingBox& List, PhysicBody* Pb, bool IsTrigger  );
// 	void FinalizeActor(NxActorDesc ActorDesc, ListOfBoundingBox List, bool IsTrigger);
// 	NxMaterialIndex GenMaterial( float restitution, float staticFriction, float dynamiqueFriction );
// 
// public:
// 	BoundingBox(){}																									//TRIGGER
// 	BoundingBox(ShapeDescription* Desc);
// 	BoundingBox(ListOfBoundingBox &BBList, bool IsTrigger);
// 	BoundingBox& operator =(const BoundingBox& bb);
// 
// 	~BoundingBox(){}
// };

int CreateBoundingBox(ShapeDescription* Desc);
int CreateBoundingBox(ListOfBoundingBox &BBList, bool IsTrigger = false);

void CreateBox( NxActorDesc& ActorDesc, ListOfBoundingBox& List, PhysicBody* Pb, bool IsTrigger );
void CreateSphere( NxActorDesc& ActorDesc, ListOfBoundingBox& List, PhysicBody* Pb, bool IsTrigger  );
void CreateCapsule( NxActorDesc& ActorDesc, ListOfBoundingBox& List, PhysicBody* Pb, bool IsTrigger  );
void FinalizeActor(NxActorDesc ActorDesc, ListOfBoundingBox List, bool IsTrigger);
NxMaterialIndex GenMaterial( float restitution, float staticFriction, float dynamiqueFriction );						//TRIGGER

#endif
