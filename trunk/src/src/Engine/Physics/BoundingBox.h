#ifndef		_BOUNDINGBOX_H
#define		_BOUNDINGBOX_H

#include	<list>

#include	"NxPhysics.h"
#include	"NxBoxController.h"
#include	"NxCapsuleController.h"
#include	"NxControllerManager.h"

#include	"Resources/Mesh.h"
#include	"Trigger/UserData.h"

class Hero;
class SceneObject;
enum GroupCollision;

//Type de la shape d'une bounding box
enum ShapeType
{
	BOX,
	SPHERE,
	CAPSULE,
	LOAD_ERROR
};

//Type de l'objet
/*enum PhysicalObjectType
{
	PHYS_DYNAMIQUE,
	PHYS_STATIC,
	PHYS_TRIGGER,
	PHYS_WEAPON,
	PHYS_HERO,
	PHYS_ENEMY,
	PHYS_NOPHYSICAL
};*/


//Structure pour r�cup�rer les infos sur le body pendant le loader
struct PhysicBody
{
	ShapeType	shapeType;
	bool		IsDynamic;
	D3DXVECTOR3	bodySize; //BOX = largeur hauteur longueur
								  //SPHERE = Rayon 0 0
								  //CAPSULE = rayon, hauteur 0

	D3DXVECTOR3 globalPos; //Position locale de la bounding box
	D3DXVECTOR3 localPos; //Position locale de la bounding box
	NxQuat		rotate;    //Rotation locale de la bounding box
	D3DXVECTOR3 angularVelocity; //Vitesse de rotation � la cr�ation
	D3DXVECTOR3 linearVelocity;	 //Vitesse de d�placement � la cr�ation
	float		fMass;			 //Masse
	float		fLinearDamping;
	float		fAngularDamping;
	float		frestitution;
	float		fstaticFriction;
	float		fdynamiqueFriction;


	//Fonctions li�es aux triggers, � remplir a la cr�ation du sceneobj
	void (*OnEnterFunc)(void* param); 
	void (*OnLeaveFunc)(void* param); 
	void (*OnStayFunc)(void* param); 
};

//Liste des bounding box d'un sceneobj
class ListOfBoundingBox
{
private:
	std::vector<PhysicBody*>	m_PbList;
	std::vector<NxShapeDesc*>	m_ShapeRefList; //Ne sert qu'� la destruction.
	Vector3f m_InitialWorldPos;

public:
	void ReleaseList();					//Permet la lib�ration des descripteur qui st cr��s dynamiquement

	inline std::vector<PhysicBody*> getPbList(){ return m_PbList; }
	inline Vector3f					getInitialWorldPos(){ return m_InitialWorldPos; }

	inline void						setPbList(std::vector<PhysicBody*> L){ m_PbList = L; }
	inline void						setInitialWorldPos(Vector3f P){ m_InitialWorldPos = P; }

	inline void						pushShapeRef(NxShapeDesc *const&Desc){ m_ShapeRefList.push_back(Desc); }

};

inline NxVec3 VecToNxVec(Vector3f V){ return NxVec3(V.x, V.y, V.z); }

namespace physX
{
	void CreateBoundingBox(ListOfBoundingBox &BBList, NxActor* &pActor);
	void CreateTrigger(ListOfBoundingBox &BBList,
						void (*OnEnterFunc)(void* param),
						void (*OnLeaveFunc)(void* param), 
						void (*OnStayFunc)(void* param), void* paramEnter, void* paramLeave, void* paramStay,
						NxActor* &pActor);
	void CreateControlledCapsule( Vector3f pos, float radius, float height, void* Ref,
											NxActor* &pActor, NxController* &pController );
	void CreateControlledBox( Vector3f const pos, float width, float height, float depth, 
										void* Ref, NxActor* &pActor, NxController* &pController ); 

	/************************************************************************************
	* D�truit un acteur ainsi que son userdata
	************************************************************************************/
	void releaseActor(NxActor* &pAct);
	
	/************************************************************************************
	* D�truit un controller ainsi que son userdata
	************************************************************************************/
	void releaseController(NxController* &pCont);

	/************************************************************************************
	* Fonction de r�cup�ration de la sc�ne physique
	* @return	NxScene* : sc�ne physique
	************************************************************************************/
	NxScene* getPhysicScene();

	/************************************************************************************
	* Fonction de r�cup�ration du manager des controlleur
	* @return	NxControllerManager* : manager des controlleur
	************************************************************************************/
	NxControllerManager* getControllerManager();
	
	/************************************************************************************
	* Fonction qui permet de lier deux objets de sorte � pouvoir r�cup�rer leurs collisions
	* @param[in]	obj1 : Pointeur sur le premier objet
	* @param[in]	obj2 : Pointeur sur le deuxieme objet
	************************************************************************************/
	void Link( SceneObject* const obj1, SceneObject* const obj2 );
}

#endif
