#ifndef		_BOUNDINGBOX_H
#define		_BOUNDINGBOX_H

#include	<list>
#include	"NxPhysics.h"
#include	"Resources/Mesh.h"
#include	"Physicalizer.h"

enum ShapeType
{
	BOX,
	SPHERE,
	CAPSULE,
	PLAN
};

NxVec3 VecToNxVec(const Vector3f V);
void Normalize(Vector3f &V);
float Norme(Vector3f V);

//////////////////////////////////////////////////////////////////////////
//Interface des classes de description des Shape. la classe 
//ShapeDescritption contient les paramêtres physiques des BB. Non exhaustif
//////////////////////////////////////////////////////////////////////////
struct BoundingDescription
{
	//Initialisation des variables non indispensables
	void CommonInit()
	{
		initialvelocity = Vector3f(0.f, 0.f, 0.f);
		mass = 1.0f;
	}

	//Constructeur BOX
	BoundingDescription(Vector3f aDimension = Vector3f(1.0f, 1.0f, 1.0f),
						Vector3f aglobalPosition = Vector3f(0.0f, 0.0f, 0.0f),
						float aDensity = 1.0f)
			: density( aDensity ), globalPosition( aglobalPosition ), dimension(aDimension){CommonInit();}

	//Constructeur SPHERE
	BoundingDescription(float aRadius,
						Vector3f aglobalPosition = Vector3f(0.0f, 0.0f, 0.0f),
						float aDensity = 1.0f)
			: density( aDensity ), globalPosition( aglobalPosition ), radius(aRadius){CommonInit();}
	//Constructeur CAPS
	BoundingDescription(float aRadius,
					float aHeight,
					Vector3f aglobalPosition = Vector3f(0.0f, 0.0f, 0.0f),
					float aDensity = 1.0f)
			: density( aDensity ), globalPosition( aglobalPosition ), radius(aRadius), height(aHeight){CommonInit();}
	//Constructeur PLAN
	BoundingDescription(Vector2f aSurface,
					Vector3f aglobalPosition = Vector3f(0.0f,0.0f, 0.f))
			: globalPosition( aglobalPosition ), surface(aSurface){CommonInit();}

//Variables de ShapeDesc
	float    density;		//[0, inf[ def : 0
	float    mass;			//[0, inf[ def : 0
	Vector3f globalPosition;
	Vector3f initialvelocity;

//Variables de BoxDesc
	Vector3f dimension;
//Variables de SphereDesc
	float radius;
//Variables de CapsuleDesc
	float height;
//Variables de PlaneDesc
	Vector2f surface;
};

//
////Les structures qui suivent sont particulières à chaque shapes
//struct BoxDescription : public ShapeDescription
//{
//	BoxDescription(Vector3f aDimension = Vector3f(1.0f, 1.0f, 1.0f), float aDensity = 1.0f, float aMass = 1.0f, Vector3f aglobalPosition = Vector3f(0.0f, 0.0f, 0.0f)) 
//		: ShapeDescription( aDensity, aMass, aglobalPosition), dimension( aDimension)
//	{ }
//	BoxDescription(ShapeDescription* SD, Vector3f aDimension = Vector3f(0.0f, 0.0f, 0.0f))
//		: ShapeDescription(SD->density, SD->mass, SD->globalPosition), dimension( aDimension)
//	{ }
//
//	Vector3f dimension;
//};
//
//struct SphereDescription : public ShapeDescription
//{
//	SphereDescription(float aRadius = 1.0f, float aDensity = 1.0f, float aMass = 1.0f, Vector3f aglobalPosition = Vector3f(0.0f, 0.0f, 0.0f)) 
//		: ShapeDescription( aDensity, aMass, aglobalPosition), radius( aRadius)
//	{ }
//	float radius;
//};
//
//struct CapsuleDescription : public ShapeDescription
//{
//	CapsuleDescription(float aRadius = 1.0f, float aHeight = 1.0f, float aDensity = 1.0f, float aMass = 1.0f, Vector3f aglobalPosition = Vector3f(0.0f, 10.0f, 0.0f)) 
//		: ShapeDescription( aDensity, aMass, aglobalPosition), radius( aRadius ), height( aHeight)
//	{ }
//	float radius, height;
//};
//
//struct GroundDescription : public ShapeDescription
//{
//	GroundDescription(Vector2f asurface, Vector3f aglobalPosition = Vector3f(0.0f, 10.0f, 0.0f)) 
//		: ShapeDescription( 0.f, 0.f, aglobalPosition), surface(asurface)
//	{ }
//	Vector2f surface;
//};

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
	BoundingBox BoundingBoxInit(BoundingDescription *Desc, ShapeType Type = BOX, MaterialPhysics Mat = MaterialPhysics());

public:
	//Constructeur par défaut
	BoundingBox(){}
	//Constructeurs spécifiques, simplifiées
	BoundingBox(Vector3f adimension			, Vector3f aglobalpos, float adenstity, Vector3f InitVelocity = Vector3f(0.f, 0.f, 0.f), MaterialPhysics aMat = MaterialPhysics()); //BOX
	BoundingBox(float aradius				, Vector3f aglobalpos, float adenstity, Vector3f InitVelocity = Vector3f(0.f, 0.f, 0.f), MaterialPhysics aMat = MaterialPhysics()); //SPHERE
	BoundingBox(float aradius, float aheight, Vector3f aglobalpos, float adenstity, Vector3f InitVelocity = Vector3f(0.f, 0.f, 0.f), MaterialPhysics aMat = MaterialPhysics()); //CAPS
	BoundingBox(Vector2f asurface			, float Hauteur,																		 MaterialPhysics aMat = MaterialPhysics()); //SOL
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
