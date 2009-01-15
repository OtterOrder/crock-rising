#include "CameraTPS.h"

CameraTPS::CameraTPS( Vector3f pos )
:Camera(pos)
{

}

void CameraTPS::RotationAutourPoint(Vector3f point, float angle)
{
	D3DXMATRIX t1, rot, t2, tmp;
	
	D3DXMatrixRotationY( &rot, angle );						 //Rotation Y a l'origine
	D3DXMatrixTranslation( &t1, point.x, -25.0f , point.z ); //Translation de O vers point de l'Obj

	//m_Position.x, m_Position.y, m_Position.z -> Offset Camera
	D3DXMatrixTranslation( &t2, m_Position.x, m_Position.y, m_Position.z ); //Translation Offset Camera 

	D3DXMatrixMultiply( &tmp, &rot, &t1 );
	D3DXMatrixMultiply( &m_MatrixView, &t2, &tmp );

}
