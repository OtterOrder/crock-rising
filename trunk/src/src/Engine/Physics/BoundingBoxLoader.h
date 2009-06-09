#ifndef		_BoundingBoxLoader_h_
#define		_BoundingBoxLoader_h_

/************** Include **************************/

#include	<vector>
#include	<cstring>
#include	<Resources/Resource.h>
#include	<TinyXml/tinyxml.h>
#include	<Resources/AnimLoader.h>  //Pour utiliser la fonction ConvertStringToFloatArray

#include	"NxQuat.h"

/***********************************************/
enum ShapeType
{
	BOX,
	SPHERE,
	CAPSULE,
	PLAN,
	TRIGGER,
	LOAD_ERROR
};

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
	bool		bDisableCollision;
	float		restitution;
	float		staticFriction;
	float		dynamiqueFriction;

	//Fonctions liées aux triggers, à remplir a la création du sceneobj
	void (*OnEnterFunc)(); 
	void (*OnLeaveFunc)(); 
	void (*OnStayFunc)(); 
};

void ConvertStringToFloatArrayV2(const char * Array, float * FloatArray, int iCount);
void ConvertStringToFloatArrayV3(const char * Array, float * FloatArray, int iCount);
struct NodeSaver
{
	TiXmlNode *library_visual_scenes;
	TiXmlNode *library_physics_materials;
	TiXmlNode *physics_model;
	TiXmlNode *RigidBodyNode;

	int NumShape;
};

/************************************************************************************/
//Classe qui récupère les infos nécessaires aux données physique
/************************************************************************************/
class BoundingBoxLoader 
{

	ResourceResult fillPhysicBody		( NodeSaver NodeSave, bool Dyn ); //numShape est utilisé pour la pos
	ResourceResult getVelocity			( NodeSaver NodeSave ); 
	ResourceResult getLocalPos			( NodeSaver NodeSave ); 
	ResourceResult getMaterial			( NodeSaver NodeSave ); 
	ResourceResult getSize				( TiXmlNode* nodeShape ); 
	ResourceResult getDampingAndType	( TiXmlNode* nodeShape ); 
	ResourceResult getMass				( TiXmlNode* nodeShape ); 
public:
	BoundingBoxLoader(){}
	~BoundingBoxLoader(){}

	ResourceResult Load				  ( const std::string resource );

	std::vector<PhysicBody*> getvDynamicBody () const { return m_vDynamicBody; }

protected:
	std::vector<PhysicBody*> m_vDynamicBody; //vector contenant les bounding box d'objets dynamiques

};

#endif