#ifndef		_GameCamera_H
#define		_GameCamera_H

//******************************************************************

#include	"Objects/Camera.h"

//******************************************************************

class GameCamera : public Camera
{
public:

	GameCamera( Vector3f position )
		:Camera( position ){}

	void Update();

protected:

	//TODO

};

//******************************************************************
#endif		_GameCamera_H
