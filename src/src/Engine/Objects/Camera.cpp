#include "Camera.h"

Camera::Camera( Vector3f pos, Vector3f target, Vector3f up )
:Object( pos )
{ 
	InitAngle();

	//SetPosition( pos );
	m_Offset = pos;

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

void Camera::SetTarget( Object* obj  )
{
	Vector3f pos = obj->GetPosition();
	SetTarget( pos );
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
	D3DXMATRIX offset, target, rotY, rotX;
	
	D3DXMatrixRotationY( &rotY, D3DXToRadian( m_vAngleY ) );	//selon axeY
	D3DXMatrixRotationX( &rotX, D3DXToRadian( m_vAngleX ) );	//selon axeX

	Vector3f vTarget = GetTarget();

	D3DXMatrixTranslation( &offset, -m_Offset.x, -m_Offset.y, -m_Offset.z );
	D3DXMatrixTranslation( &target, -m_Target.x, -m_Target.y, -m_Target.z );

	D3DXMatrixIdentity( &m_MatrixView );
	m_MatrixView = target * rotY * rotX * offset; 

	D3DXMatrixInverse( &m_WorldMatrix, NULL, &m_MatrixView );

}

void Camera::SetOrientationY( float fAngleY )	//en degres
{ 
	m_vAngleY += fAngleY;

	if( abs(m_vAngleY) > 359 )	//limitation de l'angleY
		m_vAngleY = 0;
}

void Camera::SetOrientationX( float fAngleX )	//en degres
{ 
	m_vAngleX += fAngleX;

	if( m_vAngleX > 0 )			//limitation de l'angleY
		m_vAngleX = 0.f;
	else
	{
		if( m_vAngleX < -60.f )
			m_vAngleX = -60.f;
	}
}

Vector3f Camera::GetPosition()
{
	return Vector3f( m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43 );
}

void Camera::InitAngle()
{ 
	m_vAngleX = m_vAngleY = 0; 
}