#ifndef		_Anim_H
#define		_Anim_H

//******************************************************************

#include	"Resource.h"
#include    "AnimLoader.h"

//******************************************************************

class Anim : public Resource 
{
public : 
	Anim () {}
	virtual ~Anim() {} 

	ResourceResult Load ( std::string resource , ResourceParam param ) ; 

	ResourceResult    FillMatrices(); 
	void			  ReleaseMatrices();

};

//******************************************************************
#endif		// _Anim_H
