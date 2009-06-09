#ifndef		_ControledPhysicalCharacter_H
#define		_ControledPhysicalCharacter_H

#include	"NxPhysics.h"
#include	"NxBoxController.h"
#include	"NxCapsuleController.h"
#include	"Core/Types/Vector.h"
#include	"Physics/BoundingBoxLoader.h"

class HeroHitReport : public NxUserControllerHitReport
{
public:
	virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit);
	virtual NxControllerAction  onControllerHit(const NxControllersHit& hit);
};

class AlienHitReport : public NxUserControllerHitReport
{
public:
	virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit);
	virtual NxControllerAction  onControllerHit(const NxControllersHit& hit)
	{ 
		return NX_ACTION_NONE; 
	}
};

int CreateControlledCapsule( Vector3f pos, float radius, float height );
int CreateControlledBox( Vector3f pos, Vector3f size );


// class ControledPhysicalCharacter : public SceneObject, public ControledBB
// {
// 	bool m_isAlien;
// 
// public:
// 	ControledPhysicalCharacter(const std::string& mesh, /*const std::string& anim,*/ const D3DXVECTOR3& Position)
// 		:SceneObject/*Animated*/( mesh, /*anim,*/ Position ), ControledBB( Position ), m_isAlien(false)
// 	{
// 	}
// 
// 	//Le booléen ne sert à rien, juste pour appelé un autre ctor
// 	ControledPhysicalCharacter(const std::string& mesh, /*const std::string& anim,*/ const D3DXVECTOR3& Position, bool Alien )
// 		:SceneObject/*Animated*/( mesh, /*anim,*/ Position ), ControledBB( Position, BOX )
// 	{
// 		m_isAlien = true; //Alien
// 	}
// 
// 	Vector3f updateControlledPosition( void );
// 
// 	void Update();
// 
// };


#endif
