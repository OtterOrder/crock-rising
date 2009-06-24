#ifndef		_SceneObject_H
#define		_SceneObject_H

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	<map>
#include	<vector>
#include	"Object.h"
#include	"Resources/Texture.h"
#include	"../Physics/BoundingBox.h"
#include	"../Physics/ControledPhysicalCharacter.h"


//===========================================================================//

class Texture;
class Shader;
class Mesh;
class Material;
//class BoundingBox;
class Renderer;
class ShadowMap;

//===========================================================================//
// Classe pour un objet affichable dans la scène 3D                          //
//===========================================================================//
class SceneObject : public Object
{
	friend class Renderer;
	friend class ShadowMap;

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
	void			SetCastShadow(bool value);
	void			SetReceiveShadow(bool value);
	virtual Mesh*	GetMesh ()	{ return m_pMesh; };


	//===========================================================================//
	// Physique de l'objet														 //
	//===========================================================================//
	void SetObjectPhysical( const std::string& physic, PhysicalObjectType type = PHYS_DYNAMIQUE );		
	void SetControledCharacter(float radius, float height, void* Ref, PhysicalObjectType type = PHYS_HERO );	
	void SetControledCharacter(float width, float height, float depth, void* Ref, PhysicalObjectType type = PHYS_HERO );	
	void SetObjectTrigger( const std::string& physic,
							void (*OnEnterFunc)(void* param), void (*OnStayFunc)(void* param), void (*OnLeaveFunc)(void* param),
							void* paramEnter, void* paramLeave, void* paramStay); 
	void SetObjectUnPhysical();												// Retire la physique à un objet si besoin
	
	int  getEmpActor()				{ return m_iEmpActor; }
	int  getEmpController()			{ return m_iEmpController; }
	std::vector<PhysicBody*> getPhysicBodyList();

	bool IsController()	{ return m_iEmpController != -1; }			//Le controleur est un acteur ET un controleur
	bool IsActor()		{ return m_iEmpActor != -1 && !IsController(); }	//L'acteur n'est qu'un acteur.
	bool IsPhysical()	{ return IsActor() ||IsController(); }
	bool IsDynamic()	{ return (*getPhysicBodyList().begin())->physObjType == PHYS_DYNAMIQUE; }
public:

	//===========================================================================//
	// Utilisation de l'objet													 //
	//===========================================================================//

	void			Init();
	virtual void	SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj);
	virtual void	SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos);
	virtual void	SetTransform(const D3DXMATRIX* world);
	virtual void	ApplyTransform(const D3DXMATRIX* world);
	virtual void    Update(); // Méthode appelée chaque tour moteur
	virtual void    SetTranslation( float x, float y, float z );
	virtual void    SetRotation( float angleX, float angleY, float angleZ );
	

	std::string getStringMesh () const { return m_strMesh; }   //Nécessaire pour les méthodes addWeapon et removeWeapon de la classe Hero
	



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
	Shader*							m_pShadowShader;
	bool							m_bVisible;		// Visibilité de l'objet
	bool							m_bCastShadow;
	bool							m_bReceiveShadow;
	LPD3DXMATRIXSTACK				m_matrixStack;  // Stack de matrice de l'objet
	ListOfBoundingBox				m_ListOfBoundingBox;
	int								m_iEmpActor;
	int								m_iEmpController;
	

	
	//===========================================================================//
	// Méthodes privées												             //
	//===========================================================================//
	
	void			InitObject();
	virtual void	InitDeviceData();
	virtual void	Draw();
	virtual void	DeleteDeviceData();
	virtual void	DeleteData();
	virtual void	DrawShadow();
	void			CommonInit( void ); // Initialisation commune à tous les constructeurs

	//Fonctions de transformations spécifiques à physX, appelés par les fonctions publics
	void	SetPhysicalTranslation( float dispX, float dispY, float dispZ );
	void	SetPhysicalRotation( float angleX, float angleY, float angleZ );
	void	SetPhysicalTransform( const D3DXMATRIX* world );
	
};

#endif
