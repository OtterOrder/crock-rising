#ifndef		_Physicalizer_H
#define		_Physicalizer_H

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#define NOMINMAX
#include	<windows.h>

#include	"NxPhysics.h"
#include	"Timer/UpdateTime.h"
#include	"Core/Singleton.h"
#include	"../Core/Types/Vector.h"


//Structure qui contiendra tous les paramètres de l'instance physX qui ne seront pas indispensable
//afin d'avoir une utilisation plus intuitive de la classe. 
struct AdvancedPhysXParam
{
	AdvancedPhysXParam(NxReal aSkinWidth = 0.025f,			 // [0, inf[
						NxReal aVisualisationScale = 1,		 // [0 / 1]
						NxReal aVisualizeCollisionShape = 1, // [0 / 1]
						NxReal aVisualizeActorAxe = 1,		 // [0 / 1]
						NxReal aVisualizeClothSleep = 0)	 // [0 / 1]
	{
		SkinWidth				= aSkinWidth;
		VisualisationScale		= aVisualisationScale;
		VisualizeCollisionShape = aVisualizeCollisionShape;
		VisualizeActorAxe		= aVisualizeActorAxe;
		VisualizeClothSleep		= aVisualizeClothSleep;
	}
	~AdvancedPhysXParam(){}

	NxReal SkinWidth;
	NxReal VisualisationScale;
	NxReal VisualizeCollisionShape;
	NxReal VisualizeActorAxe;
	NxReal VisualizeClothSleep;
};

class Physicalizer : public Singleton< Physicalizer >
{
	friend class Singleton< Physicalizer >;

	NxPhysicsSDK*		m_PhysicsSDK	;	
	NxScene*			m_Scene			;
	Vector3f			m_Gravity		;
	AdvancedPhysXParam	m_AdvancedParam ;
	NxReal				m_DeltaTime		;

public:

	bool InitPhysX();
	void ExitNx();
	void StartPhysics();
	void GetPhysicsResults(); //Doit être avant chaque rendu pour la mise à jour des matrices.

	///////////////////////////////////////////////////////////////////////////
	// Modificateurs														 //
	///////////////////////////////////////////////////////////////////////////
	NxPhysicsSDK*		getPhysicsSDK()		{ return m_PhysicsSDK;		}	
	NxScene*			getScene()			{ return m_Scene;			}	
	Vector3f			getGravity()		{ return m_Gravity;			}	
	AdvancedPhysXParam	getAdvancedParam()	{ return m_AdvancedParam ;  }	

	///////////////////////////////////////////////////////////////////////////
	// Accesseurs															 //
	///////////////////////////////////////////////////////////////////////////
	void getPhysicsSDK	  (NxPhysicsSDK* aSDK)			{ m_PhysicsSDK = aSDK;		}	
	void getScene		  (NxScene*		aScene)			{ m_Scene = aScene;			}	
	void getGravity		  (Vector3f		aGravity)		{ m_Gravity = aGravity;		}	
	void getAdvancedParam (AdvancedPhysXParam aParam)	{ m_AdvancedParam = aParam; }	

	void CreateCube(const NxVec3& pos, int size, const NxVec3* initialVelocity);
	void CreateCubeFromEye(int size);
	void CreateStack(int size);
	void CreateSin(int size);
	void CreateSinSuface(int Hauteur, int Duree, int Periode);
	void CreateTower(int size);

	
protected:
	
	Physicalizer(){ InitPhysX(); }
	virtual ~Physicalizer(){ ExitNx(); }
			
	HINSTANCE		m_Instance;			// Instance de l'application
};

//******************************************************************
#endif		// _Physicalizer_H