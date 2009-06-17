#ifndef		_BoundingBoxLoader_h_
#define		_BoundingBoxLoader_h_

/************** Include **************************/

#include	<vector>
#include	<cstring>
#include	<Resources/Resource.h>
#include	<TinyXml/tinyxml.h>
#include	<Physics/BoundingBox.h>

#include	"NxQuat.h"

/***********************************************/

void ConvertStringToFloatArrayV2(const char * Array, float * FloatArray, int iCount);

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
	ResourceResult getGlobalPosAndRotate( NodeSaver NodeSave ); 
	ResourceResult getLocalPos			( TiXmlNode* nodeShape ); 
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