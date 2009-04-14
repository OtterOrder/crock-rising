#pragma once
//===========================================================================//
// Include                                                                   //
//===========================================================================//
#define		NOMINMAX

#include	<map>
#include	<vector>
#include	"Object.h"
#include	"Resources/Texture.h"


//===========================================================================//

class Texture;
class Shader;
class Mesh;
class Material;
class BoundingBox;
typedef std::list< int > TEmpList;

//===========================================================================//
// Classe pour un objet affichable dans la scène 3D                          //
//===========================================================================//
class SceneObject : public Object
{
public:

	// Liste de références sur les objets de scène
	static std::list< SceneObject* > RefList;

	typedef std::list< SceneObject* >::iterator ScObjIt;

	// Constructeurs & destructeur
	SceneObject(const std::string& mesh, const D3DXVECTOR3& Position);
	SceneObject();
	virtual ~SceneObject();

	//===========================================================================//
	// Gestion apparence des objets											     //
	//===========================================================================//
	void				SetMaterial(Material * mat);
	Material*			GetMaterial() {return m_pMaterial;}
	void				SetShader(const std::string& strShader);

	//===========================================================================//
	// Visibilité de l'objet													 //
	//===========================================================================//
	virtual void	SetVisible(bool value);
	virtual bool	GetVisible();

	virtual Mesh*	GetMesh ()	{ return m_pMesh; };
public:

	//===========================================================================//
	// Utilisation de l'objet													 //
	//===========================================================================//
	void	InitObject();
	//void	Update();
	virtual void	SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj);
	virtual void	SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos);
	virtual void	SetTransform(const D3DXMATRIX* world);
	virtual void	ApplyTransform(const D3DXMATRIX* world) {m_WorldMatrix = *world;}
	virtual void	InitDeviceData();
	void	FrameMove(float fElapsedTime);
	virtual void	Draw();
	virtual void	DeleteDeviceData();
	virtual void	DeleteData();

	std::string getStringMesh () const { return m_strMesh; }   //Nécessaire pour les méthodes addWeapon et removeWeapon de la classe Hero
	
	TEmpList* getEmpList(){ return m_EmpList; }
	void setEmpList(TEmpList* L){ m_EmpList = L; }


protected:
	//===========================================================================//
	// Données membres												             //
	//===========================================================================//
	D3DXVECTOR3						m_Offset;
	std::string						m_strMesh;
	std::string						m_strTex;
	std::string						m_strShader;
	Material*						m_pMaterial;	// Matériau de l'objet
	Mesh*							m_pMesh;		// Mesh de l'objet
	Shader*							m_pShader;		// Shader de l'objet
	bool							m_bVisible;		// Visibilité de l'objet
	TEmpList*						m_EmpList;

	
	//===========================================================================//
	// Méthodes privées												             //
	//===========================================================================//
	
	void CommonInit( void ); // Initialisation commune à tous les constructeurs
	
};

