#ifndef _SCENEOBJECT_H
#define _SCENEOBJECT_H
//===========================================================================//
// Include                                                                   //
//===========================================================================//
#define		NOMINMAX

#include	<map>
#include	"Object.h"
#include	"Resources/Texture.h"
#include	"Physics/BoundingBox.h"


//===========================================================================//

class Texture;
class Shader;
class Mesh;

//===========================================================================//
// Classe pour un objet affichable dans la scène 3D                          //
//===========================================================================//
class SceneObject : public Object
{
public:

	// Liste de références sur les objets de scène
	static std::list< SceneObject* > RefList;

	// Constructeurs & destructeur
	SceneObject(const std::string& mesh,
				const std::string& Tex,
				const D3DXVECTOR3& Position,
				const std::string shader,
				BoundingBox aBoundingBox = BoundingBox());
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
	//void	Update();
	virtual void	SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj);
	virtual void	SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos);
	virtual void	SetTransform(const D3DXMATRIX* world);
	void	InitDeviceData();
	void	FrameMove(float fElapsedTime);
	virtual void	Draw();
	void	DeleteDeviceData();
	void	DeleteData();
	
protected:

	//===========================================================================//
	// Types												                     //
	//===========================================================================//
	typedef std::map<TextureType, Texture*> TTextureMap;

	//===========================================================================//
	// Données membres												             //
	//===========================================================================//
	std::string			m_Mesh;
	std::string			m_Tex;
	std::string			m_Shader;
	TTextureMap			m_MapTexture;

	
	Shader*				m_PtrShader;
	D3DXVECTOR3			m_Offset;

	Mesh*				m_PtrMesh;

	std::list< BoundingBox > BoundingBoxList;

;

	//===========================================================================//
	// Méthodes privées												             //
	//===========================================================================//
	
	void CommonInit( void ); // Initialisation commune à tous les constructeurs
	
};

#endif
