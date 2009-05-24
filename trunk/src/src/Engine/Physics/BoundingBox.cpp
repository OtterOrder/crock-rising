#include "BoundingBox.h"
#include "Physicalizer.h"


ControllerHitReport* gTest = new ControllerHitReport;

//===========================================================================//
//Testera si il y a collision avec l'objet po                                //
//===========================================================================//
bool BoundingBox::IsInCollision(const BoundingBox &po)
{
	return false;
}

NxVec3 VecToNxVec(const Vector3f V)
{
	return NxVec3(V.x, V.y, V.z);
}

void Normalize( Vector3f &V )
{
	V = V/Norme(V);
}

float Norme( Vector3f V )
{
	float norme = sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
	return norme > 0 ? norme : 1;
}

BoundingBox& BoundingBox::operator=( const BoundingBox& bb )
{
	m_bDebugMode = bb.getDebugMode();
 	m_Mat= bb.getMat();
	m_iEmplacement = bb.getEmplacement();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Création d'une boundingbox + intégration dans le moteur physique		   //
// Il faut passer la description associé au type(, boxdesc pour box etc)   //
/////////////////////////////////////////////////////////////////////////////
BoundingBox::BoundingBox(ShapeDescription* Desc)
{
	m_bDebugMode = true;

	Physicalizer* physXInstance = Physicalizer::GetInstance();

	NxActorDesc actorDesc;
	NxScene* scene = physXInstance->getScene();

	//m_Mat = Mat;

	switch( Desc->m_type )
	{
	case BOX:
		{
			NxBoxShapeDesc boxDesc;
			BoxDescription* bd = (BoxDescription*)Desc;

			boxDesc.dimensions = VecToNxVec( bd->m_dimension);
			boxDesc.group = Physicalizer::GROUP_DYNAMIQUE;
			boxDesc.mass = 0.0;//bd->m_mass;
			boxDesc.localPose.t = VecToNxVec( bd->m_ReglagePivot );

			actorDesc.shapes.push_back(&boxDesc);
			actorDesc.density		= (NxReal)bd->m_density;
			actorDesc.globalPose.t	= VecToNxVec(bd->m_pos) + NxVec3( 0.f, bd->m_dimension.y/2, 0.f );
		}
		break;

	case SPHERE:
		{
			NxSphereShapeDesc sphereDesc;
			SphereDescription* sd = (SphereDescription*)Desc;

			sphereDesc.radius = (NxReal)sd->m_radius ;
			sphereDesc.mass = sd->m_mass;
			sphereDesc.localPose.t = VecToNxVec( sd->m_ReglagePivot );

			actorDesc.shapes.push_back(&sphereDesc);
			actorDesc.density		= (NxReal)sd->m_density;	
			actorDesc.globalPose.t	= VecToNxVec(sd->m_pos) + NxVec3( 0.f, sd->m_radius/2, 0.f );
		} 
		break;

	case CAPSULE:
		{
			NxCapsuleShapeDesc capsuleDesc;
			CapsuleDescription* cd = (CapsuleDescription*)Desc;

			capsuleDesc.radius =(NxReal)cd->m_radius ;
			capsuleDesc.height =(NxReal)cd->m_height ;
			capsuleDesc.mass = cd->m_mass;
			capsuleDesc.localPose.t = VecToNxVec( cd->m_ReglagePivot );

			actorDesc.shapes.push_back(&capsuleDesc);
			actorDesc.density		= (NxReal)cd->m_density;	
			actorDesc.globalPose.t	= VecToNxVec(cd->m_pos);

		}
		break;
	case TRIGGER:
		{
			//Paramétrage de l'acteur
			NxBoxShapeDesc boxDesc;
			TriggerDescription* td = (TriggerDescription*)Desc;
			boxDesc.dimensions = VecToNxVec( td->m_dimension );
			boxDesc.shapeFlags |= NX_TRIGGER_ENABLE;

			actorDesc.shapes.pushBack(&boxDesc);
			actorDesc.globalPose.t = VecToNxVec( td->m_pos ) + NxVec3( 0.f, td->m_dimension.y/2, 0.f );

			assert(actorDesc.isValid());

			//Remplissage du userdata => on lui donne ses fonctions
			actorDesc.userData = new ActorUserData;
			((ActorUserData*)(actorDesc.userData))->OnEnterFunc = td->m_OnEnterFunc  ;
			((ActorUserData*)(actorDesc.userData))->OnLeaveFunc = td->m_OnLeaveFunc  ;
			((ActorUserData*)(actorDesc.userData))->OnStayFunc  = td->m_OnStayFunc   ;
		}
	}

	if( Desc->m_type != TRIGGER )
	{
		NxBodyDesc bodyDesc;
		bodyDesc.angularDamping = 0.5f;
		bodyDesc.linearVelocity = VecToNxVec(Desc->m_linearVelocity);
		actorDesc.body = &bodyDesc;
	}


	NxActor* pActor = scene->createActor( actorDesc );
	assert(pActor);

	//int test = scene->getNbActors();
	m_iEmplacement = scene->getNbActors() - 1;
}

////////////////////////////////////////////////////////////////////////
//////////////////////////// FUCKING MANUAL ////////////////////////////
////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//SPHERE
//BoundingBox::BoundingBox(float aradius, Vector3f aglobalpos, float adenstity, Vector3f InitVelocity, MaterialPhysics aMat)
//{
//	BoundingDescription sd(aradius, aglobalpos, adenstity);
//	sd.initialvelocity = InitVelocity;
//	*this = BoundingBoxInit(&sd, SPHERE, aMat);
//}
//
////////////////////////////////////////////////////////////////////////////
////CAPS
//BoundingBox::BoundingBox(float aradius, float aheight, Vector3f aglobalpos, float adenstity, Vector3f InitVelocity, MaterialPhysics aMat)
//{
//	BoundingDescription cd(aradius, aheight, aglobalpos, adenstity);
//	cd.initialvelocity = InitVelocity;
//	*this = BoundingBoxInit(&cd, CAPSULE, aMat);
//}
//
////////////////////////////////////////////////////////////////////////////
////SOL
//BoundingBox::BoundingBox(Vector2f asurface, float Hauteur, MaterialPhysics aMat)
//{
//	BoundingDescription gd(asurface, Vector3f(0.f, Hauteur, 0.f));
//	*this = BoundingBoxInit(&gd, PLAN, aMat);
//}

//BoundingBox& BoundingBox::operator=( const BoundingBox& bb )
//{
//	m_bDebugMode = bb.getDebugMode();
// 	m_Mat= bb.getMat();
//	m_iEmplacement = bb.getEmplacement();
//	return *this;
//}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//NxVec3 VecToNxVec(const Vector3f V)
//{
//	return NxVec3(V.x, V.y, V.z);
//}
//
//void Normalize( Vector3f &V )
//{
//	V = V/Norme(V);
//}
//
//float Norme( Vector3f V )
//{
//	float norme = sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
//	return norme > 0 ? norme : 1;
//}





ControledBB::ControledBB( Vector3f pos, ShapeType type )
{
	Physicalizer* physXInstance = Physicalizer::GetInstance();

	empControlerList = 0;

	//if( type==BOX )
	//{
	//	NxBoxControllerDesc desc;
	//	desc.position.x		= pos.x, desc.position.y = pos.y, desc.position.z = pos.z;
	//	//desc.extents		= gInitialExtents * scale;
	//	desc.upDirection	= NX_Y;
	//	desc.slopeLimit		= 0;
	////	desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
	//	desc.skinWidth		= 0.2f;
	//	desc.stepOffset		= 0.5;
	//	//	desc.stepOffset	= 0.01f;
	//	//		desc.stepOffset		= 0;
	//	//		desc.stepOffset		= 10;
	//	//desc.callback		= &gControllerHitReport;
	//	//gManager->createController(&scene, desc);
	//	physXInstance->m_ControllerManager->createController( physXInstance->getScene(), desc );
	//	empControlerList	= physXInstance->m_ControllerManager->getNbControllers() - 1;
	//	empActorList		= physXInstance->getScene()->getNbActors() - 1;

	//}
	//else
	{
		if( type==CAPSULE )
		{
			NxCapsuleControllerDesc desc;
			desc.radius			= 1.0f; //4.5f;//gInitialRadius * scale;
			desc.height			= 1.5f; //15.f;//gInitialHeight * scale;

			desc.position.x = pos.x ,//- 0.7538f , 
			desc.position.y = pos.y + desc.height/2.0f + desc.radius,//- 3.822f, 
			desc.position.z = pos.z ;//+ 25.4295f;

			desc.upDirection	= NX_Y;
			//		desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
			desc.slopeLimit		= 0.707f;
			desc.skinWidth		= 0.01f;
			desc.stepOffset		= 0.5f;
			//desc.stepOffset		= gInitialRadius * 0.5 * scale;
			//	desc.stepOffset	= 0.01f;
			//		desc.stepOffset		= 0;	// Fixes some issues
			//		desc.stepOffset		= 10;

			desc.callback		= gTest;

			physXInstance->m_ControllerManager->createController( physXInstance->getScene(), desc );
			empControlerList	= physXInstance->m_ControllerManager->getNbControllers() - 1;
			empActorList		= physXInstance->getScene()->getNbActors() - 1;
		}
	}

}

void ControledBB::moveTo( float dispX, float dispY, float dispZ )
{
	Physicalizer* physXInstance = Physicalizer::GetInstance();
	NxController* pControler = physXInstance->m_ControllerManager->getController( empControlerList );

	NxU32 collisionFlags;
	NxF32 minDistance = 0.001f;
	pControler->move( NxVec3( dispX, dispY, dispZ ), Physicalizer::MASK_OTHER, 
					  minDistance, collisionFlags );

	//Collision par un coté
	//if( !(collisionFlags & NXCC_COLLISION_DOWN) )
	/*{
		NxExtendedVec3 pos = pControler->getPosition();
		pos.y -= 0.01f;
		pControler->setPosition( pos );	
	}*/
	/*else 
	{
		if( collisionFlags & NXCC_COLLISION_UP )
		{
			collisionFlags=0;	
		}
	}*/


}

NxController* ControledBB::getBBController()
{
	Physicalizer* physXInstance = Physicalizer::GetInstance();
	physXInstance->m_ControllerManager->updateControllers();
	return physXInstance->m_ControllerManager->getController( empControlerList );
}

void ControledPhysicsCharacter::updateControlledPosition( void )
{
	NxController* pControler = getBBController();
	NxExtendedVec3 pos = pControler->getPosition();
	m_vPosition = Vector3f( (float)pos.x , 
							(float)pos.y /*- 1.f*/,
							(float)pos.z );
}

SceneObjectPhysics::SceneObjectPhysics(const std::string& mesh, const D3DXVECTOR3& Position)
	:SceneObject( mesh, Position ) 
{
	BoundingBox Boite = BoundingBox( &SphereDescription(0.5, 1.0, 1.0, Vector3f(0.0, 5.0, 0.0)) );
	Physicalizer* physInstance = Physicalizer::GetInstance();
	physInstance->SetPhysicable( this, &Boite );	
}

SceneObjectPhysics::SceneObjectPhysics(const std::string& mesh, const std::string& physic, const D3DXVECTOR3& Position)
	:SceneObject( mesh, Vector3f(0.f, 0.f, 0.f) ) 
{
	m_PhysicPath = physic;
	m_PhysicPosition = Position;
}

void SceneObjectPhysics::InitObject()
{
	SceneObject::InitObject();
	BoundingBoxLoader Loader;
	std::vector<DynamicBody*> BB_List;
	DynamicBody* BB; 

	if( Loader.Load(m_PhysicPath) == RES_SUCCEED)
	{
		BB_List = Loader.getvDynamicBody();
		BB = BB_List[0];
		BoundingBox Boite;
		switch(BB->type)
		{
			case BOX :
				{		
					BoxDescription SD = BoxDescription(BB->bodySize, 1.0, BB->fMass, BB->translate + m_PhysicPosition);
					m_pMesh->m_ReglagePivot;
					SD.m_ReglagePivot = m_pMesh->m_ReglagePivot;
					Boite = BoundingBox( &SD );
					break; 
				}

			case SPHERE :
				{	
					SphereDescription SD = SphereDescription(BB->bodySize.x, 1.0, BB->fMass, BB->translate + m_PhysicPosition);
					m_pMesh->m_ReglagePivot;
					SD.m_ReglagePivot = m_pMesh->m_ReglagePivot;
					Boite = BoundingBox( &SD );
					break;
				}

			case CAPSULE :
				{	
					CapsuleDescription SD = CapsuleDescription(BB->bodySize.x, BB->bodySize.y, 1.0, BB->fMass, BB->translate + m_PhysicPosition);
					m_pMesh->m_ReglagePivot;
					SD.m_ReglagePivot = m_pMesh->m_ReglagePivot;
					Boite = BoundingBox( &SD );
					break; 
				}
			case LOAD_ERROR :{ break; }

			default:		break;
		}
		Physicalizer* physInstance = Physicalizer::GetInstance();
		physInstance->SetPhysicable( this, &Boite );	
	}	
}