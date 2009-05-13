#ifndef		_BOUNDINGBOX_H
#define		_BOUNDINGBOX_H

#include	<list>

#define	NOMINMAX

#include	"NxPhysics.h"
#include	"NxBoxController.h"
#include	"NxCapsuleController.h"
#include	"NxCooking.h"

#include	"Resources/Mesh.h"
#include	"Trigger/UserData.h"
#include	"Objects/SceneObject.h"
#include	"Objects/SceneObjectAnimated.h"


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

class ControllerHitReport : public NxUserControllerHitReport
{
public:
	virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit)
	{
		if(hit.shape)
		{
			NxCollisionGroup group = hit.shape->getGroup();
			if( group /*!= 0*/ /*Physicalizer::GROUP_STATIC*/ )
			{
				NxActor& actor = hit.shape->getActor();
				if(actor.isDynamic())
				{
					if(hit.dir.y==0.0f)
					{
						NxF32 force = 200.0f;
						NxF32 coeff = actor.getMass() * hit.length * force;
						actor.addForceAtLocalPos( hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE );
					}
				}
			}
			/*else
			{
				NxActor& actor = hit.shape->getActor();
			}*/
		}

		return NX_ACTION_NONE;
	}

	virtual NxControllerAction  onControllerHit(const NxControllersHit& hit)
	{ 
		return NX_ACTION_NONE; 
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


class SceneObjetPhysics : public SceneObject, public BoundingBox
{
public:
	SceneObjetPhysics(const std::string& mesh, const D3DXVECTOR3& Position);
};


class ControledBB
{
	int empControlerList;
	int empActorList;

	ControllerHitReport objCallback;

protected:
	NxController* getBBController( void );

public:
	ControledBB( Vector3f pos, ShapeType type=CAPSULE );

	void moveTo( float dispX, float dispY, float dispZ );

};


class ControledPhysicsCharacter : public SceneObject/*Animated*/, public ControledBB
{
public:
	ControledPhysicsCharacter(const std::string& mesh, /*const std::string& anim,*/ const D3DXVECTOR3& Position)
		:SceneObject/*Animated*/( mesh, /*anim,*/ Position ), ControledBB( Position )
	{}

	void updateControlledPosition( void );

};

class PhysicsStaticMesh : public SceneObject//, public ControledBB
{
public:
	PhysicsStaticMesh(const std::string& mesh, const D3DXVECTOR3& Position)
		:SceneObject( mesh, Position )//, ControledBB( Position )
	{
		NxTriangleMeshDesc terrainDesc;
		/*terrainDesc.numVertices					= gTerrainData->nbVerts;
		terrainDesc.numTriangles				= gTerrainData->nbFaces;*/
		terrainDesc.pointStrideBytes			= sizeof(NxVec3);
		terrainDesc.triangleStrideBytes			= 3*sizeof(NxU32);
	/*	terrainDesc.points						= gTerrainData->verts;
		terrainDesc.triangles					= gTerrainData->faces;*/
		terrainDesc.flags						= 0;

		terrainDesc.heightFieldVerticalAxis		= NX_Y;
		terrainDesc.heightFieldVerticalExtent	= -1000.0f;

		//MemoryWriteBuffer buf;
		//bool status =  Physicalizer::m_Cooking->NxCookTriangleMesh( terrainDesc, buf );
		//if (!status) 
		//{
		//	printf("Unable to cook a triangle mesh.");
		//	exit(1);
		//}

		//MemoryReadBuffer readBuffer( buf.data );
		//terrainMesh = Physicalizer::m_PhysicsSDK->createTriangleMesh(readBuffer);

		//
		// Please note about the created Triangle Mesh, user needs to release it when no one uses it to save memory. It can be detected
		// by API "meshData->getReferenceCount() == 0". And, the release API is "gPhysicsSDK->releaseTriangleMesh(*meshData);"
		//

		/*NxTriangleMeshShapeDesc terrainShapeDesc;
		terrainShapeDesc.meshData				= terrainMesh;
		terrainShapeDesc.shapeFlags				= NX_SF_FEATURE_INDICES;

		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&terrainShapeDesc);
		gTerrain = gScene->createActor(ActorDesc);
		gTerrain->userData = (void*)0;*/

		//Physicalizer::m_Cooking->
	
	}


};










#endif
