#include "Alien.h"

/***********************************************************
* Constructeur
***********************************************************/
Alien::Alien(Vector3f pos)
: Enemy( pos )
{
	m_idBone = 22;
}

/**********************************************************
* Initialisation des données membres
**********************************************************/
void Alien::Init()
{
	Enemy::Init();

	m_pAnimated = new SceneObjectAnimated("Mesh_Alien.DAE","Anim_Alien_Walk.DAE",m_Position);
	m_pAnimated->Init();
	m_pAnimated->GetMaterial()->SetTexture("alien.jpg",Texture::DIFFUSE);
	m_pAnimated->GetMaterial()->SetTexture("alien_normal.png",Texture::NORMALMAP);
	m_pAnimated->SetShader("default_skinnormalmap.fx");
	m_pAnimated->SetLoop(true);
	m_pAnimated->SetAnimFPS(50.f);
	
	m_pArme = new SceneObject( "dummy.dae", Vector3f( 5, 18, 0)); //en fn de la pos de m_pAnimated
	m_pArme->Init();
	m_pArme->SetObjectPhysical( "arme_mmegrise.DAE" ); //pas le bon group!!

	InitWeapon();
}

void Alien::Hit()
{
	Enemy::Hit();
	//playsound TODO
}

void Alien::Die()
{
	Enemy::Die();
	//playsound TODO
}

void Alien::InitSound()
{

}

void Alien::InitAnim()
{
	AddAnim(Perso::RUN,		"Anim_Alien_Walk.DAE"	, 50.f, true);
	AddAnim(Perso::ATTACK,	"Anim_Alien_Attack.DAE"	, 50.f, false);
	AddAnim(Perso::HIT,		"Anim_Alien_Hit.DAE"	, 50.f, false);
	AddAnim(Perso::DIE,		"Anim_Alien_Die.DAE"	, 50.f, false);
	AddAnim(Perso::STATIC,	"Anim_Alien_Wait.DAE"	, 50.f, true);
}