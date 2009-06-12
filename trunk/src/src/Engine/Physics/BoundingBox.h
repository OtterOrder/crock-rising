#ifndef		_BOUNDINGBOX_H
#define		_BOUNDINGBOX_H

#include	<list>

#include	"NxPhysics.h"
#include	"NxBoxController.h"
#include	"NxCapsuleController.h"
#include	"NxControllerManager.h"

#include	"Resources/Mesh.h"
#include	"Trigger/UserData.h"

//Type de la shape d'une bounding box
enum ShapeType
{
	BOX,
	SPHERE,
	CAPSULE,
	TRIGGER,
	LOAD_ERROR
};

//Type de l'objet
enum PhysicalObjectType
{
	ACTOR,
	CONTROLLER,
	NOPHYSICAL
};

//Structure pour r�cup�rer les infos sur le body pendant le loader
struct PhysicBody
{
	ShapeType   type;
	D3DXVECTOR3 bodySize; //BOX = largeur hauteur longueur
						  //SPHERE = Rayon 0 0
						  //CAPSULE = rayon, hauteur 0

	D3DXVECTOR3 translate; //Position locale de la bounding box
	NxQuat		rotate;    //Rotation locale de la bounding box
	D3DXVECTOR3 angularVelocity; //Vitesse de rotation � la cr�ation
	D3DXVECTOR3 linearVelocity;	 //Vitesse de d�placement � la cr�ation
	float		fMass;			 //Masse
	float		fLinearDamping;
	float		fAngularDamping;
	bool		bIsDynamic;
	float		restitution;
	float		staticFriction;
	float		dynamiqueFriction;

	//Fonctions li�es aux triggers, � remplir a la cr�ation du sceneobj
	void (*OnEnterFunc)(); 
	void (*OnLeaveFunc)(); 
	void (*OnStayFunc)(); 
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
	void MajPivot(const Mesh* pMesh);	

	inline std::vector<PhysicBody*> getPbList(){ return m_PbList; }
	inline Vector3f					getInitialWorldPos(){ return m_InitialWorldPos; }

	inline void						setPbList(std::vector<PhysicBody*> L){ m_PbList = L; }
	inline void						setInitialWorldPos(Vector3f P){ m_InitialWorldPos = P; }

	inline void						pushShapeRef(NxShapeDesc *const&Desc){ m_ShapeRefList.push_back(Desc); }

};

inline NxVec3 VecToNxVec(Vector3f V){ return NxVec3(V.x, V.y, V.z); }

namespace physX
{
	/************************************************************************************
	* Fonction de cr�ation d'une bounding box.
	* @param[in]	ListOfBoundingBox	: Liste des bounding box du DAE avec leurs param
	* @return		int : l'indice de l'emplacement de l'acteur
	************************************************************************************/
	int CreateBoundingBox(ListOfBoundingBox &BBList);

	/*************************************************************************************
	* Fonction de cr�ation d'un trigger.
	* @param[in]	ListOfBoundingBox	: Liste des bounding box du DAE avec leurs param
	* @param[in]	OnEnterFunc	: Fonction lanc�e quand un objet RENTRE dans le trigger
	* @param[in]	OnLeaveFunc	: Fonction lanc�e quand un objet SORT du le trigger
	* @param[in]	OnStayFunc	: Fonction lanc�e quand un objet EST dans le trigger
	* @return		int : l'indice de l'emplacement du trigger
	************************************************************************************/
	int CreateTrigger(ListOfBoundingBox &BBList,
		void (*OnEnterFunc)(), 
		void (*OnLeaveFunc)(), 
		void (*OnStayFunc)());

	/************************************************************************************
	* Fonction de cr�ation d'une capsule controller
	* @param[in]	pos	: Position initiale en world, provient du SceneObject
	* @param[in]	radius	: Rayon de la capsule du controller
	* @param[in]	height	: hauteur du controller
	* @return		int : l'indice de l'emplacement du controller
	* Pr�condition	radius et height ne doivent pas �tre tous les deux nuls.
	************************************************************************************/
	int CreateControlledCapsule( Vector3f pos, float radius, float height, int &empActor );

	/************************************************************************************
	* Fonction de cr�ation d'une box controller
	* @param[in]	pos	: Position initiale en world, provient du SceneObject
	* @param[in]	size: largeur, hauteur et profondeur de la box 
	* @return		int : l'indice de l'emplacement du controller
	* Pr�condition	size ne doivent pas �tre un vecteur nul.
	************************************************************************************/
	int CreateControlledBox( Vector3f pos, Vector3f size, int &empActor );

	/************************************************************************************
	* Fonction de r�cup�ration de la sc�ne physique
	* @return	NxScene* : sc�ne physique
	************************************************************************************/
	NxScene* getPhysicScene();

	/************************************************************************************
	* Fonction de r�cup�ration d'un acteur
	* @param[in]	emp	: emplacement de l'acteur dans la liste des acteurs de physX
	* @return		NxActor* : acteur
	* Pr�condition	emp doit �tre strictement positif et inf�rieur au nombre d'acteur.
	************************************************************************************/
	NxActor* getActor(int emp);

	/************************************************************************************
	* Fonction de r�cup�ration du manager des controlleur
	* @return	NxControllerManager* : manager des controlleur
	************************************************************************************/
	NxControllerManager* getControllerManager();

	/************************************************************************************
	* Fonction de r�cup�ration d'un controleur
	* @param[in]	emp	: emplacement du controleur dans la liste des controleur de physX
	* @return		NxController* : controleur
	* Pr�condition	emp doit �tre strictement positif et inf�rieur au nombre de controller.
	************************************************************************************/
	NxController* getController(int emp);
}

#endif
