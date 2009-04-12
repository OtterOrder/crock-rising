#ifndef		_BoundingBoxLoader_h_
#define		_BoundingBoxLoader_h_

/************** Include **************************/

#include	<vector>
#include	<cstring>
#include	<Resources/Resource.h>
#include	<TinyXml/tinyxml.h>
#include	<Resources/AnimLoader.h>  //Pour utiliser la fonction ConvertStringToFloatArray

/***********************************************/
enum typeShape
{
	BOX,
	SPHERE,
	CAPSULE
};

struct StaticBody
{
	typeShape   type;
	D3DXVECTOR3 bodySize;
	D3DXVECTOR3 translate;
	D3DXVECTOR3 rotate;
	bool		bDisableCollision;
};

struct DynamicBody
{
	typeShape   type;
	D3DXVECTOR3 bodySize;
	D3DXVECTOR3 translate;
	D3DXVECTOR3 rotate;
	float		fMass;
	float		fLinearDamping;
	float		fAngularDamping;
	bool		bDisableCollision;
};


/************************************************************************************/
//Classe qui récupère les infos nécessaires aux données physique
/************************************************************************************/
class BoundingBoxLoader 
{
public:
	BoundingBoxLoader(){}
	~BoundingBoxLoader(){}

	ResourceResult Load				  ( const std::string resource );
	ResourceResult fillDynamicObject  ( TiXmlNode *node );
	ResourceResult fillStaticObject   ( TiXmlNode *node );

	std::vector<DynamicBody*> getvDynamicBody () const { return m_vDynamicBody; }
	std::vector<StaticBody*>  getvStaticBody()	 const { return m_vStaticBody;  }

protected:
	std::vector<DynamicBody*> m_vDynamicBody; //vector contenant les bounding box d'objets dynamiques
	std::vector<StaticBody*>  m_vStaticBody;  //vector contenant les bounding box d'objets statiques

};

#endif