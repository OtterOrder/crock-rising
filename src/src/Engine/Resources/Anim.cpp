#include "Anim.h"

ResourceResult Anim::Load(crc32 resource)
{
	char sAnimPath[128];
	sprintf_s(sAnimPath, "..\\..\\data\\test\\%u.DAE", resource);

	AnimLoader animLoader ; 
	animLoader.Load(sAnimPath);

	return RES_SUCCEED ;
}
