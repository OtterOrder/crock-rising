#ifndef		_BOUNDINGBOX_H
#define		_BOUNDINGBOX_H

#include	<list>

#include	"NxPhysics.h"
#include	"NxBoxController.h"
#include	"NxCapsuleController.h"

#include	"Resources/Mesh.h"
#include	"Trigger/UserData.h"

enum ShapeType
{
	BOX,
	SPHERE,
	CAPSULE,
	PLAN,
	TRIGGER,
	LOAD_ERROR
};

enum PhysicalObjectType
{
	ACTOR,
	CONTROLLER,
	NOPHYSICAL
};

//Structure pour récupérer les infos sur le body pendant le loader
struct PhysicBody
{
	ShapeType   type;
	D3DXVECTOR3 bodySize; //BOX = largeur hauteur longueur
						  //SPHERE = Rayon 0 0
						  //CAPSULE = rayon, hauteur 0

	D3DXVECTOR3 translate; //Position locale de la bounding box
	NxQuat		rotate;    //Rotation locale de la bounding box
	D3DXVECTOR3 angularVelocity; //Vitesse de rotation à la création
	D3DXVECTOR3 linearVelocity;	 //Vitesse de déplacement à la création
	float		fMass;			 //Masse
	float		fLinearDamping;
	float		fAngularDamping;
	bool		bIsDynamic;
	float		restitution;
	float		staticFriction;
	float		dynamiqueFriction;

	//Fonctions liées aux triggers, à remplir a la création du sceneobj
	void (*OnEnterFunc)(); 
	void (*OnLeaveFunc)(); 
	void (*OnStayFunc)(); 
};

//Liste des bounding box d'un sceneobj
class ListOfBoundingBox
{
private:
	std::vector<PhysicBody*>	m_PbList;
	std::vector<NxShapeDesc*>	m_ShapeRefList; //Ne sert qu'à la destruction.
	Vector3f m_InitialWorldPos;

public:
	void ReleaseList();					//Permet la libération des descripteur qui st créés dynamiquement
	void MajPivot(const Mesh* pMesh);	

	inline std::vector<PhysicBody*> getPbList(){ return m_PbList; }
	inline void setPbList(std::vector<PhysicBody*> L){ m_PbList = L; }
	inline Vector3f getInitialWorldPos(){ return m_InitialWorldPos; }
	inline void setInitialWorldPos(Vector3f P){ m_InitialWorldPos = P; }
	inline void pushShapeRef(NxShapeDesc *const&Desc){ m_ShapeRefList.push_back(Desc); }

};

NxVec3 VecToNxVec(Vector3f V);
void Normalize(Vector3f &V);
float Norme(const Vector3f V);

//Fonction qui permet de créer une bounding box.
int CreateBoundingBox(ListOfBoundingBox &BBList);
int CreateTrigger(ListOfBoundingBox &BBList,
	void (*OnEnterFunc)(), 
	void (*OnLeaveFunc)(), 
	void (*OnStayFunc)());

void CreateBox( NxActorDesc& ActorDesc, ListOfBoundingBox& List, const PhysicBody* Pb, const bool IsTrigger );
void CreateSphere( NxActorDesc& ActorDesc, ListOfBoundingBox& List, const PhysicBody* Pb, const bool IsTrigger  );
void CreateCapsule( NxActorDesc& ActorDesc, ListOfBoundingBox& List, const PhysicBody* Pb, const bool IsTrigger  );
void FinalizeActor(const NxActorDesc ActorDesc, ListOfBoundingBox List, const bool IsTrigger);
NxMaterialIndex GenMaterial( const float restitution, const float staticFriction, const float dynamiqueFriction );						//TRIGGER

#endif
