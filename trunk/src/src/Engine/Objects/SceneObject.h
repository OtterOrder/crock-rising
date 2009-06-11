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
// Classe pour un objet affichable dans la sc�ne 3D                          //
//===========================================================================//
class SceneObject : public Object
{
	friend class Renderer;
	friend class ShadowMap;

public:

	// Liste de r�f�rences sur les objets de sc�ne
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
	// Visibilit� de l'objet													 //
	//===========================================================================//
	virtual void	SetVisible(bool value);
	virtual bool	GetVisible();
	void			SetCastShadow(bool value);
	void			SetReceiveShadow(bool value);
	virtual Mesh*	GetMesh ()	{ return m_pMesh; };


	//===========================================================================//
	// Physique de l'objet														 //
	//===========================================================================//
	void SetObjectPhysical( const std::string& physic, Vector3f Pos);		// Donne la main au moteur physique pour g�rer l'objet
	void SetControledCharacter(Vector3f Pos, float radius, float height);	// Fournis une capsule � un controlleur physique
	void SetControledCharacter(Vector3f Pos, Vector3f size);				// ---------- box ------------------------------
	void SetObjectUnPhysical();
	void SetObjectTrigger( const std::string& physic, Vector3f Pos, void (*OnEnterFunc)(), void (*OnStayFunc)(), void (*OnLeaveFunc)());
	
	void SetPhysicalTranslation( float dispX, float dispY, float dispZ );
	int  getEmpActor(){ return m_EmpActor; }
	void setEmpActor(int Emp){ m_EmpActor = Emp; }
	int  getEmpController(){ return m_EmpController; }
	void setEmpController(int Emp){ m_EmpController = Emp; }
	std::vector<PhysicBody*> getPhysicBodyList();
	bool IsActor(){ return m_EmpActor != -1; }
	bool IsController(){ return m_EmpController != -1; }
public:

	//===========================================================================//
	// Utilisation de l'objet													 //
	//===========================================================================//

	void			Init();
	virtual void	SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj);
	virtual void	SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos);
	virtual void	SetTransform(const D3DXMATRIX* world);
	virtual void	BerSetTransform(const D3DXMATRIX* world);
	virtual void	ApplyTransform(const D3DXMATRIX* world);
	virtual void    Update(); // M�thode appel�e chaque tour moteur
	virtual void    SetTranslation( float x, float y, float z );
	virtual void    SetRotation( int angleX, int angleY, int angleZ );
	

	std::string getStringMesh () const { return m_strMesh; }   //N�cessaire pour les m�thodes addWeapon et removeWeapon de la classe Hero
	



protected:
	//===========================================================================//
	// Donn�es membres												             //
	//===========================================================================//
	D3DXVECTOR3						m_Offset;
	std::string						m_strMesh;
	std::string						m_strTex;
	std::string						m_strShader;
	Material*						m_pMaterial;	// Mat�riau de l'objet
	Mesh*							m_pMesh;		// Mesh de l'objet
	Shader*							m_pShader;		// Shader de l'objet
	Shader*							m_pShadowShader;
	bool							m_bVisible;		// Visibilit� de l'objet
	bool							m_bCastShadow;
	bool							m_bReceiveShadow;
	LPD3DXMATRIXSTACK				m_matrixStack;  // Stack de matrice de l'objet
	ListOfBoundingBox				m_ListOfBoundingBox;
	int								m_EmpActor;
	int								m_EmpController;
	

	
	//===========================================================================//
	// M�thodes priv�es												             //
	//===========================================================================//
	
	void			InitObject();
	virtual void	InitDeviceData();
	virtual void	Draw();
	virtual void	DeleteDeviceData();
	virtual void	DeleteData();
	virtual void	DrawShadow();
	void			CommonInit( void ); // Initialisation commune � tous les constructeurs
	
};

#endif
