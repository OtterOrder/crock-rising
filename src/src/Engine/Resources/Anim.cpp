#include "Anim.h"

ResourceResult Anim::Load(std::string resource , ResourceParam param)
{
	std::string str="..\\..\\data\\test\\"+resource;

	AnimLoader animLoader(str.c_str()); 
	animLoader.Load(str.c_str());

	return RES_SUCCEED ;
}

