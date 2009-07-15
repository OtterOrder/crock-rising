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
	void SetObjectPhysical( const std::string& physic );		
	void SetControledCharacter(float radius, float height, void* Ref );	
	void SetControledCharacter(float width, float height, float depth, void* Ref );	
	void SetObjectTrigger( const std::string& physic,
							void (*OnEnterFunc)(void* param), void (*OnStayFunc)(void* param), void (*OnLeaveFunc)(void* param),
							void* paramEnter, void* paramLeave, void* paramStay); 
	void SetObjectUnPhysical();												// Retire la physique à un objet si besoin
	
	std::vector<PhysicBody*> getPhysicBodyList();
	NxActor* getActor()				{ return m_pActor; }
	NxController* getController()	{ return m_pController; }

	bool IsController()	{ return m_pController != NULL; }			//Le controleur est un acteur ET un controleur
	bool IsActor()		{ return m_pActor != NULL && !IsController(); }	//L'acteur n'est qu'un acteur.
	bool IsPhysical()	{ return IsActor() || IsController(); }
	bool IsDynamic()	{ return (*getPhysicBodyList().begin())->IsDynamic; }
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
	virtual void	SetPosition( float x, float y, float z );
	virtual void    SetTranslation( float x, float y, float z );
	virtual void	SetOrientation( float angleX, float angleY, float angleZ );
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
	float							m_iHauteurController; //Sert à descendre le sceobj lorsque c'est un controller
	NxActor*						m_pActor;
	NxController*					m_pController;
	

	
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

private:
	//Fonctions de transformations spécifiques à physX, appelés par les fonctions publics
	void	SetPhysicalPosition( float x, float y, float z );
	void	SetPhysicalTranslation( float x, float y, float z );
	void	SetPhysicalOrientation( float angleX, float angleY, float angleZ );
	void	SetPhysicalRotation( float angleX, float angleY, float angleZ );
	void	SetPhysicalTransform( const D3DXMATRIX* world );	
};

#endif
