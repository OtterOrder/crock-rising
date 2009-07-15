#include "MmeGrise.h"
#include "../AIToolkit/AIManager.h"

/***********************************************************
* Constructeur
***********************************************************/
MmeGrise::MmeGrise(Vector3f pos)
: Enemy( pos )
{
	m_idBone = 14;
}

/**********************************************************
* Initialisation des données membres
**********************************************************/
void MmeGrise::Init()
{
	Enemy::Init();

	m_pAnimated = new SceneObjectAnimated("Mesh_Mmegrise.DAE","Anim_Mmegrise_Walk.DAE",m_Position);
	m_pAnimated->Init();
	m_pAnimated->GetMaterial()->SetTexture("mmegrise_diffuse.jpg",Texture::DIFFUSE);
	m_pAnimated->SetLoop(true);
	m_pAnimated->SetAnimFPS(50.f);

	m_pArme = new SceneObject( "dummy.dae", Vector3f( 5, 18, 0)); //en fn de la pos de m_pAnimated
	m_pArme->Init();
	m_pArme->SetObjectPhysical( "arme_mmegrise.DAE" ); //pas le bon group!!

	InitWeapon();
}

void MmeGrise::Hit()
{
	Enemy::Hit();
	//playsound TODO
}

void MmeGrise::Die()
{
	Enemy::Die();
	//playsound TODO
}

void MmeGrise::InitSound()
{
	AddSound(Perso::RUN,	"BTFM_High.ogg");
	AddSound(Perso::ATTACK, "Attack1.ogg");
	AddSound(Perso::ATTACK, "Attack2.ogg");
	AddSound(Perso::ATTACK, "Attack3.ogg");
	AddSound(Perso::HIT,	"Hit1.ogg");
	AddSound(Perso::HIT,	"Hit2.ogg");
	AddSound(Perso::HIT,	"Hit3.ogg");
	AddSound(Perso::DIE,	"Die1.ogg");
	AddSound(Perso::DIE,	"Die2.ogg");
	AddSound(Perso::DIE,	"Die3.ogg");
}

void MmeGrise::InitAnim()
{
	AddAnim(Perso::RUN,		"Anim_Mmegrise_Walk.DAE"	, 30.f, true);
	AddAnim(Perso::ATTACK,	"Anim_Mmegrise_Attack.DAE"	, 30.f, false);
	AddAnim(Perso::HIT,		"Anim_Mmegrise_Hit.DAE"		, 30.f, false);
	AddAnim(Perso::DIE,		"Anim_Mmegrise_Die.DAE"		, 30.f, false);
	AddAnim(Perso::STATIC,	"Anim_Mmegrise_Wait.DAE"	, 30.f, true);
}