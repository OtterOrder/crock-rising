#include "Perso.h"
#include "Hero.h"
#include <iostream>


void ContactReportCR::onContactNotify( NxContactPair& pair, NxU32 events )
{
	ActorUserData *UserDataA, *UserDataV;
 	NxActor* Arme = pair.actors[1], *Victime = pair.actors[0];

	//Vérification de la présence des acteurs
	if(Arme && Victime)
	{
		UserDataA = (ActorUserData*)(Arme->userData);
		UserDataV = (ActorUserData*)(Victime->userData);
		Hero* pHero = (Hero*)UserDataA->PersoRef;
		if( pHero->getCurrentState() == ATTACK )
		{
			//Si l'objet a bien un userdata
			if(UserDataA && UserDataV)
			{
				//Si l'objet qui frappe est une arme
				if( UserDataA->type == WEAPON )
				{
					Perso* perso = (Perso*)UserDataV->PersoRef;
					perso->Hit();
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
	SceneObject::RefList.remove(m_pAnimated);
	m_pAnimated->SetObjectUnPhysical();
 	delete m_pAnimated;
 	m_pAnimated = NULL;
}
