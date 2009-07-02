#include "Perso.h"
#include "Hero.h"
#include <iostream>


void ContactReportCR::onContactNotify( NxContactPair& pair, NxU32 events )
{
	ActorUserData *ud1, *ud2;
 	NxActor* Arme = pair.actors[1], *Victime = pair.actors[0];

	//Vérification de la présence des acteurs
	if(Arme && Victime)
	{
		ud1 = (ActorUserData*)(Arme->userData);
		ud2 = (ActorUserData*)(Victime->userData);
		//Si l'objet a bien un userdata
		if(ud1 && ud2)
		{
			Perso* perso1 = (Perso*)ud1->PersoRef;
			Perso* perso2 = (Perso*)ud2->PersoRef;
			if( perso1->getCurrentState() == ATTACK )
			{
				//Si l'objet qui frappe est une arme
				if( ud1->type == WEAPON )
				{
					perso2->Hit();
				}
			}
			else if( perso2->getCurrentState() == ATTACK )
			{
				//Si l'objet qui frappe est une arme
				if( ud2->type == WEAPON )
				{
					perso1->Hit();
				}
			}
		}
	}
}


void Perso::decLife( const int decreaseLife ) 
{	 
	std::cout << "vie = " << m_iLife << std::endl;
	if ( (m_iLife - decreaseLife) <= 0 )
		Die();
	else 
		m_iLife -= decreaseLife;
}

void Perso::DestroyPerso()
{
	//SceneObject::RefList.remove(m_pAnimated);
	m_pAnimated->SetObjectUnPhysical();
 //	delete m_pAnimated;
 //	m_pAnimated = NULL;
	m_pAnimated->SetVisible(false);
	IsAlive() = false;
}
