#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "NxPhysics.h"

//Structure qui contiendra tous les paramètres de l'instance physX qui ne seront pas indispensable
//afin d'avoir une utilisation plus intuitive de la classe. 
struct AdvancedPhysXParam
{

};

class PhysX
{
public:
	PhysX(){ m_PhysicsSDK = NULL; m_Scene = NULL; }
	~PhysX(){ ExitNx(); }

	bool	InitPhysX();

private:

	NxPhysicsSDK*		m_PhysicsSDK ;
	NxScene*				m_Scene	 ;
	AdvancedPhysXParam			mAdvancedParam;

	void ExitNx();
	void CreateCube(const NxVec3& pos, int size, const NxVec3* initialVelocity);
	void HeavyCreateCube(const NxVec3& pos, int size, const NxVec3* initialVelocity);
	void CreateCubeFromEye(int size);
	void CreateStack(int size);
	void CreateSin(int size);
	//Hauteur : Hauteur des pics en Y
	//Duree	  : Nombre de cube en X et en Z du premier étage
	//Période : Nombre de bosses
	void CreateSinSuface(int Hauteur, int Duree, int Periode);
	void CreateTower(int size);
};