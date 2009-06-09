#include "AIRess.h"

int AIRess::calculDistance( Vector3f pos1, Vector3f pos2 )
{
	return (int)(sqrt(pow((pos2.x-pos1.x),2) + pow((pos2.y-pos1.y),2) + pow((pos2.z-pos1.z),2)));
}