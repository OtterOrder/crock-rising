#include "Perso.h"


void ContactReportCR::onContactNotify( NxContactPair& pair, NxU32 events )
{
	ActorUserData *UserDataA, *UserDataV;
 	NxActor* Arme = pair.actors[1], *Victime = pair.actors[0];

	//V�rification de la pr�sence des acteurs
	if(Arme && Victime)
	{
		UserDataA = (ActorUserData*)(Arme->userData);
		UserDataV = (ActorUserData*)(Victime->userData);
		//Si l'objet a bien un userdata
		if(UserDataA && UserDataV)
		{
			//Si l'objet qui frappe est une arme
			if( UserDataA->type == WEAPON )
			{
				Perso* perso = (Perso*)UserDataV->PersoRef;
				perso->Life() += 10;	
			}//endif weapon
		}//endif userdata		
	}//endif actors
}
