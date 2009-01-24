#ifndef		_Camera_H
#define		_Camera_H

#include	"Object.h"

#define DEFAULT_ZNEAR	0.01f
#define DEFAULT_ZFAR	500.0f
#define DEFAULT_RATIO	1.0f
#define DEFAUT_FOV_DEG	45

class Camera : public Object
{
public:
	//Constructeur
	Camera( Vector3f pos = Vector3f(0.0f, 0.0f, 0.0f), 
			Vector3f target = Vector3f( 0.0f, 0.0f, 1.0f ),
			Vector3f up = Vector3f( 0.0f, 1.0f, 0.0f ) );

	//Accesseurs
	Vector3f GetUp();
	Vector3f GetTarget();

	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjMatrix();

	//Modificateurs
	void SetUp( Vector3f up );
	void SetTarget( Vector3f target );
	void SetTarget( Object* obj  );		//cible un objet

	void SetZNearFar( float zn=DEFAULT_ZNEAR, float zf=DEFAULT_ZFAR );
	void SetRatio( float ratio=DEFAULT_RATIO );
	void SetFOV( int FovDeg=DEFAUT_FOV_DEG );

	void SetPosition( Vector3f pos );
	void SetOrientationY( int angleY );
	void SetOrientationX( int angleX );

	//Accesseurs
	float GetFOV(){		return m_fov_rad; };
	float GetZNear(){	return m_zNear; };
	float GetZFar(){	return m_zFar; };
	float GetRatio(){	return m_ratio; };

	Vector3f GetPosition();
	float	GetOrientationYRad(){ return D3DXToRadian( m_angleY ); }
	float	GetOrientationXRad(){ return D3DXToRadian( m_angleX ); }

	//Fonctions membres
	void InitAngle();
	void SetDefaultProjection();	// Applique les paramètres par defaut pour les variables de projection
	void UpdateMatrixView();		// Mise à jour de la matrice View en fontion des paramètres		

private:
	//Variables pour matrice de visualisation
	Vector3f m_Up;
	Vector3f m_Target;

	//Variables pour matrice de projection
	float m_fov_rad, m_ratio, m_zNear, m_zFar;

	//orientation
	int m_angleX, m_angleY; //en deg

protected :
	D3DXMATRIX m_MatrixView;		//Matrice de visualisation
	D3DXMATRIX m_MatrixProjection;	//Matrice de projection

};

#endif		// _Camera_H
