#ifndef		_Physicalizer_H
#define		_Physicalizer_H

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#define NOMINMAX
#include	<windows.h>
#include	"NxPhysics.h"
#include	"Core/Singleton.h"

//Structure qui contiendra tous les paramètres de l'instance physX qui ne seront pas indispensable
//afin d'avoir une utilisation plus intuitive de la classe. 
struct AdvancedPhysXParam
{

};

class Physicalizer : public Singleton< Physicalizer >
{
	friend class Singleton< Physicalizer >;

	NxPhysicsSDK*		m_PhysicsSDK ;
	NxScene*			m_Scene	 ;
	AdvancedPhysXParam	m_AdvancedParam;

public:

	bool InitPhysX();
	void ExitNx();


	///////////////////////////////////////////////////////////////////////////
	// Fonctions de test													 //
	///////////////////////////////////////////////////////////////////////////
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