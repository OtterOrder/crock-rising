#ifndef		_CameraTPS_H
#define		_CameraTPS_H

#include	"Camera.h"

class CameraTPS : public Camera
{

public:
	CameraTPS( Vector3f a );
	
	//FIXME : le nom et c une version de test
	void RotationAutourPoint(Vector3f point, float angle); 
	
	D3DXMATRIX GetMatrixView()
	{
		//
		return m_MatrixView;
	}


};

#endif		// _CameraTPS_H
