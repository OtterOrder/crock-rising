#ifndef		_Physicalizer_H
#define		_Physicalizer_H

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#define NOMINMAX
#include	<windows.h>
#include	<stdio.h>

#include	"NxPhysics.h"
#include	"Core/Singleton.h"
#include	"../Core/Types/Vector.h"
#include	"../Objects/SceneObject.h"
#include	"BoundingBox.h"



enum PhysXResult
{
	PHYSX_SUCCEED,
	PHYSX_FAILED
};

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
	void ExitPhysX();
	bool ReloadPhysX();
	void StartPhysics(); 
	void GetPhysicsResults(); //Doit être avant chaque rendu pour la mise à jour des matrices.
	PhysXResult DoTransform(); //Applique les transformations à tous la liste des SceneObject.
	PhysXResult RunPhysics(); //Permet de récupérer le résultat de la physique, en appelant les fonctions dans le bon ordre

	PhysXResult SetPhysicable(SceneObject* SceObj, BoundingBox* bb = NULL); //Applique une BB au SO, si la BB est nulle, on retire la physique du SO
	bool IsPhysicable(SceneObject* SceObj);

	///////////////////////////////////////////////////////////////////////////
	// Accesseurs														 //
	///////////////////////////////////////////////////////////////////////////
	NxPhysicsSDK*		getPhysicsSDK()		{ return m_PhysicsSDK;		}	
	NxScene*			getScene()			{ return m_Scene;			}	
	Vector3f			getGravity()		{ return m_Gravity;			}	
	AdvancedPhysXParam	getAdvancedParam()	{ return m_AdvancedParam ;  }

	///////////////////////////////////////////////////////////////////////////
	// Modificateurs															 //
	///////////////////////////////////////////////////////////////////////////
	void setPhysicsSDK	  (NxPhysicsSDK* aSDK)			{ m_PhysicsSDK = aSDK;		}	
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