#ifndef		_CameraFPS_H
#define		_CameraFPS_H

#include	"Camera.h"

class CameraFPS : public Camera
{

public:
	CameraFPS( void );
	CameraFPS( Vector3f pos ); 

	void SetOrientation(){}; //cad angle Y local
	void SetPivotTete(){};   //cad angle X local
	void SetPencheTeteAngle(){}; //cad angle Y local

private:

};

#endif		// _CameraFPS_H
