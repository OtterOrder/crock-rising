#include "PhysicalObject.h"


//===========================================================================//
//PhysicalObject  par défaut                                                 //
//===========================================================================//
PhysicalObject::PhysicalObject(void)
{
	m_bDebugMode = false;
}



PhysicalObject::~PhysicalObject(void)
{
}

//===========================================================================//
//Testera si il y a collision avec l'objet po                                //
//===========================================================================//
bool PhysicalObject::IsInCollision(const PhysicalObject &po)
{
	return false;
}

//===========================================================================//
//Modificateur du DegugMode					                                 //
//===========================================================================//
void PhysicalObject::EnableDebugMode(bool enable)
{}

BoundingBox::BoundingBox(Mesh* aPrimitive, NxMaterial* aMat)
{ 
	pPrimitive = aPrimitive; 
	pMat = aMat; 

	if( pMat == NULL)
	{
		//TODO : Classe abstraite, comment faire si elle est nulle?
		pMat->setRestitution(0.5);
		pMat->setStaticFriction(0.5);
		pMat->setDynamicFriction(0.5);
	}
}