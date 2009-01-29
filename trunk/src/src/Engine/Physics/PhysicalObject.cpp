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
	//release de la liste
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