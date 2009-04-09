#ifndef		_BOUNDINGBOX_H
#define		_BOUNDINGBOX_H

#include	<list>
#include	"NxPhysics.h"
#include	"Resources/Mesh.h"
#include	"Trigger/UserData.h"

enum ShapeType
{
	BOX,
	SPHERE,
	CAPSULE,
	PLAN,
	TRIGGER
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
	{ m_linearVelocity = Vector3f(0.f, 0.f, 0.f); }

	float m_density, m_mass;
	Vector3f m_pos;
	Vector3f m_linearVelocity;
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


////////////////////////////////////////////////////////////////////////////
// Structure material comprenant toutes les valeurs utiles de NxMaterials //
////////////////////////////////////////////////////////////////////////////
struct MaterialPhysics
{
	MaterialPhysics(float aRestitution = 0.5,
		     float aStaticFriction = 0.5,
			 float aDynamicFriction = 0.5)
	{
		Restitution = aRestitution;
		StaticFriction = aStaticFriction;
		DynamicFriction = aDynamicFriction;
	}
	~MaterialPhysics(){}

	float Restitution;
	float StaticFriction;
	float DynamicFriction;

	MaterialPhysics& operator =(const MaterialPhysics& m)
	{
		Restitution = m.Restitution;
		StaticFriction = m.StaticFriction;
		DynamicFriction = m.DynamicFriction;
		return *this;
	}
};

/////////////////////////////////////////////////////////
// Structure BoundingBox qui englobera les objets.	   //
// Elles seront fournies au moteur physique.		   //
/////////////////////////////////////////////////////////
class BoundingBox
{
	bool			m_bDebugMode; //Si vrai, les bounding box se dessinent pour debugger.
	MaterialPhysics		m_Mat; 
	int				m_iEmplacement;
	//What else? Groupe de collision?

public:
	BoundingBox(){}																									//TRIGGER
	BoundingBox(ShapeDescription* Desc);
	BoundingBox& operator =(const BoundingBox& bb);

	~BoundingBox(){}

	bool IsInCollision(const BoundingBox &po);

	bool getDebugMode	() const { return m_bDebugMode; }
	MaterialPhysics getMat		() const { return m_Mat; }
	int getEmplacement	() const { return m_iEmplacement; }

	void setDebugMode	(bool DebugMode) { m_bDebugMode = DebugMode; }
	void setMat			(MaterialPhysics Mat)	 { m_Mat = Mat; }
	void setEmplacement	(int Emp)		 { m_iEmplacement = Emp; }
};

#endif
