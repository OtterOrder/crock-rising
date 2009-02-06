#include	"Mask.h"

//******************************************************************

//**********************************************************
// Constructeur.
// @param[in]	sizeX : taille horizontale
// @param[in]	sizeY : taille verticale
// @param[in]	color : couleur
//**********************************************************
Mask::Mask( float sizeX, float sizeY, const Color4f &color )
: Object2D()
{
	m_Size.x	= sizeX;
	m_Size.y	= sizeY;
	m_Color		= color;
}

//**********************************************************
// Constructeur.
// @param[in]	size : taille
// @param[in]	color : couleur
//**********************************************************
Mask::Mask( const Vector2f &size, const Color4f &color )
: Object2D()
{
	m_Size.x	= size.x;
	m_Size.y	= size.y;
	m_Color		= color;
}

//**********************************************************
// Destructeur.
//**********************************************************
Mask::~Mask()
{
}

//**********************************************************
// Affiche l'objet.
//**********************************************************
void Mask::Draw() const
{
	//TODO
}
