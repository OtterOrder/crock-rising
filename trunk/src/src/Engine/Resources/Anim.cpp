#include "Anim.h"


ResourceResult Anim::Load(std::string resource , ResourceParam param)
{
	std::string str="..\\..\\data\\test\\"+resource;

	AnimLoader animLoader(str.c_str()); 
	animLoader.Load(str.c_str(),m_fBonesMatrice,m_fTimeValues,m_fBindShapeMatrix,m_fBindPosesArray);
	return RES_SUCCEED ;
}

