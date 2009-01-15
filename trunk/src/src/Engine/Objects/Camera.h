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
	Camera( void );
	Camera( Vector3f pos, 
			Vector3f target=Vector3f( 0.0f, 0.0f, 1.0f ),
			Vector3f up=Vector3f( 0.0f, 1.0f, 0.0f ) );

	//Accesseurs
	Vector3f GetUp();
	Vector3f GetTarget();

	D3DXMATRIX GetMatrixView();
	D3DXMATRIX GetMatrixProjection();

	//Modificateurs
	void SetUp( Vector3f up );
	void SetTarget( Vector3f target );
	void SetTarget( Object* obj  );		//cible un objet

	void SetZNearFar( float zn=DEFAULT_ZNEAR, float zf=DEFAULT_ZFAR );
	void SetRatio( float ratio=DEFAULT_RATIO );
	void SetFOV( int FovDeg=DEFAUT_FOV_DEG );

	//Accesseurs
	float GetFOV(){		return m_fov_rad; };
	float GetZNear(){	return m_zNear; };
	float GetZFar(){	return m_zFar; };
	float GetRatio(){	return m_ratio; };
	

	//Fonctions membres
	void SetDefaultProjection();		//Applique les paramètres par default pour les variables de projection

private:
	//Variables pour matrice de visualisation
	Vector3f m_Up;
	Vector3f m_Target;

	//Variables pour matrice de projection
	float m_fov_rad, m_ratio, m_zNear, m_zFar;

protected :
	D3DXMATRIX m_MatrixView;		//Matrice de visualisation
	D3DXMATRIX m_MatrixProjection;	//Matrice de projection

private: //Outils
	float deg2rad( int degres ){ return degres / 180.0f * D3DX_PI; };

};

#endif		// _Camera_H
