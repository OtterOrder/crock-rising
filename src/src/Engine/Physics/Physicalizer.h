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

//#define CONNECT_VRD

enum PhysXResult
{
	PHYSX_SUCCEED,
	PHYSX_FAILED
};

enum GroupCollision
{
	GROUP_NONPHYSICS = 0,
	GROUP_STATIC = 1,
	GROUP_DYNAMIQUE = 2,
	GROUP_CONTROLLER = 3,

	MASK_STATIC			= 1 << GROUP_STATIC,
	MASK_DYNAMIQUE		= 1 << GROUP_DYNAMIQUE,
	MASK_CONTROLLER		= 1 << GROUP_CONTROLLER,

	MASK_OTHER = MASK_STATIC | MASK_DYNAMIQUE

};

class Physicalizer : public Singleton< Physicalizer >
{
	friend class Singleton< Physicalizer >;
	//friend class BoundingBox;
	friend class ControledBB;
	friend class PhysicsMesh;

	NxPhysicsSDK*		m_PhysicsSDK	;	
	NxScene*			m_Scene			;
	Vector3f			m_Gravity		;
	NxReal				m_DeltaTime		;

	NxControllerManager*		m_ControllerManager;
	NxUserAllocatorDefault*		m_Allocator;


public:

	bool InitPhysX();
	void ExitPhysX();
	bool ReloadPhysX();
	void StartPhysics(float DeltaTime); 
	void GetPhysicsResults();	//Doit être avant chaque rendu pour la mise à jour des matrices.
	PhysXResult RunPhysics(float DeltaTime);	//Permet de récupérer le résultat de la physique, en appelant les fonctions dans le bon ordre
	void connectToVRD();		//Fonction d'aide au debug, lancer le remote debuger de physX et exécuter CR. 
	//void Link( SceneObject* const obj1, SceneObject* const obj2 );

	///////////////////////////////////////////////////////////////////////////
	// Accesseurs														 //
	///////////////////////////////////////////////////////////////////////////
	inline NxScene*				getScene()			{ return m_Scene;			}	
	inline Vector3f				getGravity()		{ return m_Gravity;			}	
	inline NxControllerManager* getControllerManager(){ return m_ControllerManager; }
	inline NxPhysicsSDK*		getSDK()			{ return m_PhysicsSDK; }

	///////////////////////////////////////////////////////////////////////////
	// Modificateurs															 //
	///////////////////////////////////////////////////////////////////////////
	void setScene		  (NxScene*		aScene)			{ m_Scene = aScene;			}	
	void setGravity		  (Vector3f		aGravity)		{ m_Gravity = aGravity;		}	
	void setControllerCallback( NxUserContactReport* callback );

protected:
	
	Physicalizer(){ InitPhysX(); }
	virtual ~Physicalizer(){ ExitPhysX(); }
			
	HINSTANCE		m_Instance;			// Instance de l'application
};


//******************************************************************

#endif		// _Physicalizer_H
