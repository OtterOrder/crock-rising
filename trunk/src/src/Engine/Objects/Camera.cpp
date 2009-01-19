#include "Camera.h"

Camera::Camera()
:Object()
{
	m_angleX = m_angleY = 0;

	SetUp( Vector3f( 0.0f, 1.0f, 0.0f ) );
	SetTarget( Vector3f( m_Position.x, m_Position.y, m_Position.z+1 ) ); //cad regard vers le fond

	SetDefaultProjection();
}

Camera::Camera( Vector3f pos, Vector3f target, Vector3f up )
:Object( pos )
{ 
	m_angleX = m_angleY = 0;

	SetUp( up );
	SetTarget( target );

	SetDefaultProjection();
}

Vector3f Camera::GetUp(){
	return m_Up;
}

Vector3f Camera::GetTarget(){
	return m_Target;
}

void Camera::SetUp( Vector3f up ){
	m_Up = up;
}

void Camera::SetTarget( Vector3f target ){
	m_Target = target;
}

void Camera::SetTarget( Object* obj  ){
	SetTarget( obj->GetPosition() );
}

D3DXMATRIX Camera::GetMatrixView()
{
	//D3DXMatrixLookAtLH( &m_MatrixView, &GetPosition(), &GetTarget(), &GetUp() );
	return m_MatrixView;
}

D3DXMATRIX Camera::GetFillMatrixProjection()
{
	D3DXMatrixPerspectiveFovLH( &m_MatrixProjection, m_fov_rad, m_ratio, m_zNear, m_zFar );
	return m_MatrixProjection;
}

void Camera::SetZNearFar( float zn, float zf )
{
	m_zNear = zn;
	m_zFar = zf;
}

void Camera::SetRatio( float ratio ){
	m_ratio = ratio;
}

void Camera::SetFOV( int FovDeg ){
	m_fov_rad = D3DXToRadian(FovDeg);
}

void Camera::SetDefaultProjection()
{
	SetZNearFar();
	SetRatio();
	SetFOV();
}