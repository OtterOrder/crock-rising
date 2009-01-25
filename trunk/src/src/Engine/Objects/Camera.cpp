#include "Camera.h"

Camera::Camera( Vector3f pos, Vector3f target, Vector3f up )
:Object( pos )
{ 
	InitAngle();

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
	D3DXMATRIX* transObj = obj->GetTransform();
	SetTarget( Vector3f( transObj->_41, transObj->_42, transObj->_43 ) );
}

D3DXMATRIX Camera::GetViewMatrix()
{
	return m_MatrixView;
}

D3DXMATRIX Camera::GetProjMatrix()
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

void  Camera::SetPosition( Vector3f pos )
{ 
	m_WorldMatrix._41 = pos.x;
	m_WorldMatrix._42 = pos.y;
	m_WorldMatrix._43 = pos.z;
}

void Camera::UpdateMatrixView()
{
	D3DXMATRIX position, target, rotY, rotX;
	
	D3DXMatrixRotationY( &rotY, D3DXToRadian( m_angleY ) );	//selon axeY
	D3DXMatrixRotationX( &rotX, D3DXToRadian( m_angleX ) );	//selon axeX

	Vector3f vPosition = GetPosition();
	Vector3f vTarget = GetTarget();

	D3DXMatrixTranslation( &position, -vPosition.x, -vPosition.y, -vPosition.z );
	D3DXMatrixTranslation( &target, -vTarget.x, -vTarget.y, -vTarget.z );

	D3DXMatrixIdentity( &m_MatrixView );
	m_MatrixView = target * rotY * rotX * position; 

}

void Camera::SetOrientationY( int angleY )	//en degres
{ 
	m_angleY += angleY;

	if( abs(m_angleY) > 359 )	//limitation de l'angleY
		m_angleY = 0;
}

void Camera::SetOrientationX( int angleX )	//en degres
{ 
	m_angleX += angleX;

	if( m_angleX > 0 )			//limitation de l'angleY
		m_angleX = 0;
	else
		if( m_angleX < -60 )
			m_angleX = -60;
}

Vector3f Camera::GetPosition()
{
	return Vector3f( m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43 );
}

void Camera::InitAngle()
{ 
	m_angleX = m_angleY = 0; 
}