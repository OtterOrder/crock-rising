#include "Camera.h"

Camera::Camera()
:Object()
{
	m_angleX = m_angleY = 0;

	SetUp( Vector3f( 0.0f, 1.0f, 0.0f ) );
	SetTarget( Vector3f() ); //cad regard vers le fond

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
	//SetTarget( obj->GetPosition() );
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

void  Camera::SetPosition( Vector3f pos )
{
	//D3DXMatrixTranslation( &m_WorldMatrix, -pos.x, -pos.y, -pos.z );
	m_WorldMatrix._41 = pos.x;
	m_WorldMatrix._42 = pos.y;
	m_WorldMatrix._43 = pos.z;
}

void Camera::UpdateMatrixView()
{
	D3DXMATRIX position, target, rotY, rotX;
	
	D3DXMatrixRotationY( &rotY, D3DXToRadian( m_angleY ) );	
	D3DXMatrixRotationX( &rotX, D3DXToRadian( m_angleX ) );	

	Vector3f vPosition = GetPosition();
	//Vector3f vTarget = GetTarget();

	D3DXMatrixTranslation( &position, -vPosition.x, -vPosition.y, -vPosition.z );
	//D3DXMatrixTranslation( &target, -vTarget.x, -vTarget.y, -vTarget.z );

	m_MatrixView = rotY * rotX * position; //target * rotY * rotX * position

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

	if( abs(m_angleX) > 359 )	//limitation de l'angleY
		m_angleX = 0;
}

Vector3f Camera::GetPosition()
{
	return Vector3f( m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43 );
}
