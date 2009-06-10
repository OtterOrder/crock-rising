#include	"Object2D.h"

//******************************************************************

#define		O2D_DEFAULT_POSITION		(Vector3f( 0.f, 0.f, 0.f ))
#define		O2D_DEFAULT_SCALE			(Vector3f( 1.f, 1.f, 1.f ))
#define		O2D_DEFAULT_ROTATION		(Vector3f( 0.f, 0.f, 0.f ))

//******************************************************************

std::list< Object2D* > Object2D::RefList;

//******************************************************************

//**********************************************************
// Constructeur par défaut.
//**********************************************************
Object2D::Object2D()
{
	m_Position		= O2D_DEFAULT_POSITION;
	m_Scale			= O2D_DEFAULT_SCALE;
	m_Rotation		= O2D_DEFAULT_ROTATION;
	m_Flags			= 0;
	m_Dirty			= false;
	
	// Enregistrement dans la liste
	Object2D::RefList.push_back( this );
}

//**********************************************************
// Destructeur.
//**********************************************************
Object2D::~Object2D()
{
	// Suppression de la liste
	Object2D::RefList.remove( this );
}

//**********************************************************
// Change la position.
// @param[in]	posX : Coordonnée x (pixels)
// @param[in]	posY : Coordonnée y (pixels)
//**********************************************************
void Object2D::SetPosition( s32 posX, s32 posY )
{
	if( posX != (s32)m_Position.x || posY != (s32)m_Position.y )
	{
		m_Position.x = (float)posX;
		m_Position.y = (float)posY;
		activate_dirty();
	}
}

//**********************************************************
// Change la position.
// @param[in]	position : (x,y)
//**********************************************************
void Object2D::SetPosition( const Vector2i &position )
{
	SetPosition( position.x, position.y );
}

//**********************************************************
// Change la position.
// @param[in]	position : (x,y)
//**********************************************************
void Object2D::SetPosition( const Vector2f &position )
{
	SetPosition( (s32)position.x, (s32)position.y );
}

//**********************************************************
// Donne la position. On renvoie un Vector2i car ce sont
// des pixels (donc des entiers), mais aussi parce que le
// cast Vector2i -> Vector2f est défini, contrairement au
// cast inverse.
// @return	Position du l'objet
//**********************************************************
Vector2i Object2D::GetPosition() const
{
	return Vector2i( (s32)m_Position.x, (s32)m_Position.y );
}

//**********************************************************
// Change l'échelle.
// @param[in]	scaleX : Echelle sur l'axe OX
// @param[in]	scaleY : Echelle sur l'axe OY
//**********************************************************
void Object2D::SetScale( float scaleX, float scaleY )
{
	if( scaleX != m_Scale.x || scaleY != m_Scale.y )
	{
		m_Scale.x = scaleX;
		m_Scale.y = scaleY;
		activate_dirty();
	}
}

//**********************************************************
// Change l'échelle.
// @param[in]	scale : Echelle (x,y) sur les axes (OX,OY)
//**********************************************************
void Object2D::SetScale( const Vector2f &scale )
{
	SetScale( scale.x, scale.y );
}

//**********************************************************
// Donne l'échelle.
// @return	L'echelle sur (OX,OY)
//**********************************************************
Vector2f Object2D::GetScale() const
{
	return Vector2f( m_Scale.x, m_Scale.y );
}

//**********************************************************
// Change l'angle de rotation. La rotation s'effectue autour
// du point chaud de l'objet.
// @param[in]	angle : Angle de rotation en radians
//**********************************************************
void Object2D::SetRotation( float angle )
{
	if( angle != m_Rotation.z )
	{
		m_Rotation.z = angle;
		activate_dirty();
	}
}

//**********************************************************
// Donne l'angle de rotation.
// @return	L'angle en radians
//**********************************************************
float Object2D::GetRotation() const
{
	return m_Rotation.z;
}

//**********************************************************
// Active le flag.
// @param[in]	flag : Flag à activer
//**********************************************************
void Object2D::SetFlag( u32 flag )
{
	FLAG_Set( m_Flags, flag );
}

//**********************************************************
// Désactive le flag.
// @param[in]	flag : Flag à désactiver
//**********************************************************
void Object2D::UnsetFlag( u32 flag )
{
	FLAG_Unset( m_Flags, flag );
}

//**********************************************************
// Vérifie si le flag est activé.
// @param[in]	flag : Flag à vérifier
// @return	Vrai si le flag est activé
//**********************************************************
bool Object2D::IsFlagSet( u32 flag ) const
{
	return FLAG_IsSet( m_Flags, flag );
}

//**********************************************************
// Update toute la 2D : appelle l'Update de tous les
// objets 2d, gère le "dirty", etc.
//**********************************************************
void Object2D::FullUpdate()
{
	Object2D *pObj;
	std::list<Object2D*>::iterator it = RefList.begin();
	
	while( it != RefList.end() )
	{
		pObj = *it;
		pObj->Update();
		
		if( pObj->m_Dirty )
		{
			pObj->dirty_Refresh();
			pObj->m_Dirty = false;
		}
		++it;
	}
}

//**********************************************************
// Calcule la matrice de l'objet dans le repère world.
// @param[out]	pMatrix : Matrice world de l'objet
//**********************************************************
void Object2D::WorldMatrix( D3DMATRIX *pMatrix ) const
{
	D3DXMATRIX trans, rotat, scale;
	D3DXMatrixTranslation( &trans, m_Position.x, m_Position.y, m_Position.z );
	D3DXMatrixRotationYawPitchRoll( &rotat, m_Rotation.x, m_Rotation.y, m_Rotation.z );
	D3DXMatrixScaling( &scale, m_Scale.x, m_Scale.y, m_Scale.z );
	*pMatrix = scale * rotat * trans;
}
