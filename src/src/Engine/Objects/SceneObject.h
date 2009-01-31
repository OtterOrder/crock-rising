#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	<map>
#include	"Object.h"
#include	"Resources/Texture.h"
//#include	"Physics/PhysicalObject.h"

//===========================================================================//

class Texture;
class Shader;
class Mesh;

//===========================================================================//
// Classe pour un objet affichable dans la sc�ne 3D                          //
//===========================================================================//
class SceneObject : public Object
{
public:

	// Liste de r�f�rences sur les objets de sc�ne
	static std::list< SceneObject* > RefList;

	// Constructeurs & destructeur
	SceneObject(const std::string& mesh, const std::string& Tex, const D3DXVECTOR3& Position, /*const PhysicalObject PhysicObj,*/ const std::string shader = "default.fx");
	SceneObject();
	virtual ~SceneObject();

	//===========================================================================//
	// Gestion apparence des objets											     //
	//===========================================================================//
	void	SetTexture(const std::string& Tex, TextureType Type);	// Ajout d'une texture
	void	SetShader(const std::string& Shad); 

public:

	//===========================================================================//
	// Utilisation de l'objet													 //
	//===========================================================================//
	void	InitObject();
	void	SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj);
	void	SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos);
	void	SetTransform(const D3DXMATRIX* world);
	void	InitDeviceData();
	void	FrameMove(float fElapsedTime);
	void	Draw();
	void	DeleteDeviceData();
	void	DeleteData();
	
private:

	//===========================================================================//
	// Types												                     //
	//===========================================================================//
	typedef std::map<TextureType, Texture*> TTextureMap;

	//===========================================================================//
	// Donn�es membres												             //
	//===========================================================================//
	std::string		m_Mesh;
	std::string		m_Tex;
	std::string		m_Shader;
	TTextureMap		m_MapTexture;

	
	Shader*			m_PtrShader;
	D3DXVECTOR3		m_Offset;

	Mesh*			m_PtrMesh;

	//PhysicalObject	m_PhysicObj;

	//===========================================================================//
	// M�thodes priv�es												             //
	//===========================================================================//
	
	void CommonInit( void ); // Initialisation commune � tous les constructeurs
	
};

