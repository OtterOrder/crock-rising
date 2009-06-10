#ifndef		_Physicalizer_H
#define		_Physicalizer_H

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	<stdio.h>
#include	<crtdbg.h>
#include	"NxUserAllocatorDefault.h"
#include	"NxPhysics.h"
#include	"NxControllerManager.h"


#include	"Core/Singleton.h"
#include	"../Core/Types/Vector.h"
#include	"../Objects/SceneObject.h"
#include	"BoundingBox.h"

#define CONNECT_VRD


enum PhysXResult
{
	PHYSX_SUCCEED,
	PHYSX_FAILED
};

//Structure qui contiendra tous les paramètres de l'instance physX qui ne seront pas indispensable
//afin d'avoir une utilisation plus intuitive de la classe. 
struct AdvancedPhysXParam
{
	AdvancedPhysXParam(NxReal aSkinWidth = 0.005f,			 // [0, inf[
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

NxScene* GetPhysicScene();

class Physicalizer : public Singleton< Physicalizer >
{
	friend class Singleton< Physicalizer >;
	//friend class BoundingBox;
	friend class ControledBB;
	friend class PhysicsMesh;

	NxPhysicsSDK*		m_PhysicsSDK	;	
	NxScene*			m_Scene			;
	Vector3f			m_Gravity		;
	AdvancedPhysXParam	m_AdvancedParam ;
	NxReal				m_DeltaTime		;

	NxControllerManager*		m_ControllerManager;
	NxUserAllocatorDefault*		m_Allocator;


public:
	enum GroupCollision
	{
		GROUP_NONPHYSICS = 0,
		GROUP_STATIC = 1,
		GROUP_DYNAMIQUE = 2,
		GROUP_WEAPON = 3,

		MASK_STATIC			= 1 << GROUP_STATIC,
		MASK_DYNAMIQUE		= 1 << GROUP_DYNAMIQUE,
		MASK_WEAPON			= 1 << GROUP_WEAPON,

		MASK_OTHER = MASK_STATIC | MASK_DYNAMIQUE | MASK_WEAPON

	};


	bool InitPhysX();
	void ExitPhysX();
	bool ReloadPhysX();
	void StartPhysics(); 
	void GetPhysicsResults(); //Doit être avant chaque rendu pour la mise à jour des matrices.
	PhysXResult DoTransform(); //Applique les transformations à tous la liste des SceneObject.
	PhysXResult RunPhysics(); //Permet de récupérer le résultat de la physique, en appelant les fonctions dans le bon ordre
	void connectToVRD();		//Fonction d'aide au debug, lancer le remote debuger de physX et exécuter CR.

	PhysicalObjectType IsPhysical(SceneObject* SceObj);

	///////////////////////////////////////////////////////////////////////////
	// Accesseurs														 //
	///////////////////////////////////////////////////////////////////////////
	//NxPhysicsSDK*		getPhysicsSDK()		{ return m_PhysicsSDK;		}	
	inline NxScene*			getScene()			{ return m_Scene;			}	
	inline Vector3f			getGravity()		{ return m_Gravity;			}	
	inline AdvancedPhysXParam	getAdvancedParam()	{ return m_AdvancedParam ;  }
	inline NxControllerManager* getControllerManager(){ return m_ControllerManager; }

	///////////////////////////////////////////////////////////////////////////
	// Modificateurs															 //
	///////////////////////////////////////////////////////////////////////////
	//void setPhysicsSDK	  (NxPhysicsSDK* aSDK)			{ m_PhysicsSDK = aSDK;		}	
	void setScene		  (NxScene*		aScene)			{ m_Scene = aScene;			}	
	void setGravity		  (Vector3f		aGravity)		{ m_Gravity = aGravity;		}	
	void setAdvancedParam (AdvancedPhysXParam aParam)	{ m_AdvancedParam = aParam; }	


	//Fonction récupéré dans le fichier UpdateTime.h des trainings programme de physx, en attente d'avoir notre timer a nous.
	float UpdateTime()
	{
		static unsigned int previousTime = timeGetTime();
		unsigned int currentTime = timeGetTime();
		unsigned int elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		return (float)(elapsedTime)*0.001f;
	}
	
protected:
	
	Physicalizer(){ InitPhysX(); }
	virtual ~Physicalizer(){ ExitPhysX(); }
			
	HINSTANCE		m_Instance;			// Instance de l'application
};


//******************************************************************

#endif		// _Physicalizer_H
