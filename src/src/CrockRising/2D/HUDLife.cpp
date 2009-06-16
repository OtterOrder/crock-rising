#include	"HUDLife.h"

//******************************************************************

#define		LIFE_BAR_BG				"hud_life.png"
#define		LIFE_BAR_COLOR			(Color4f( 0.12f, 0.56f, 1.f, 1.f ))
#define		LIFE_BAR_POS_X			11
#define		LIFE_BAR_POS_Y			11
#define		LIFE_BAR_MAX_WIDHT		420
#define		LIFE_BAR_HEIGHT			9

//******************************************************************

//**********************************************************
// Constructeur.
//**********************************************************
HUDLife::HUDLife()
: Sprite( LIFE_BAR_BG )
{
	m_Life		= 0;
	m_MaxLife	= 0;
	m_Scale		= 1.f;
	m_LifeBar	= NULL;
}

//**********************************************************
// Destructeur.
//**********************************************************
HUDLife::~HUDLife()
{
	if( m_LifeBar ) delete m_LifeBar;
}

//**********************************************************
// Initialise l'objet.
//**********************************************************
void HUDLife::Init()
{
	Sprite::Init();
	
	// Création de la barre..
	m_LifeBar = new Quad( LIFE_BAR_MAX_WIDHT, LIFE_BAR_HEIGHT, LIFE_BAR_COLOR );
	m_LifeBar->Init();
	m_LifeBar->SetPosition(
		GetPosition().x + (s32)(LIFE_BAR_POS_X*m_Scale),
		GetPosition().y + (s32)(LIFE_BAR_POS_Y*m_Scale)
	);
}

//**********************************************************
// Change la position.
// @param[in]	posX
// @param[in]	posY
//**********************************************************
void HUDLife::SetPosition( s32 posX, s32 posY )
{
	Sprite::SetPosition( posX, posY );
	m_LifeBar->SetPosition(
		posX + (s32)(LIFE_BAR_POS_X*m_Scale),
		posY + (s32)(LIFE_BAR_POS_Y*m_Scale)
	);
}

//**********************************************************
// Change l'échelle.
// @param[in]	scale
//**********************************************************
void HUDLife::SetScale( float scale )
{
	m_Scale = scale;
	Sprite::SetScale( m_Scale, m_Scale );
	m_LifeBar->SetScale( m_Scale, m_Scale );
}

//**********************************************************
// Change le max des points de vie.
//**********************************************************
void HUDLife::SetMaxLife( s32 maxLife )
{
	m_MaxLife	= maxLife;
	m_Life		= MATH_Clamp( m_Life, (s32)0, m_MaxLife );
	m_LifeBar	->SetWidth( GetBarWidth() );
}

//**********************************************************
// Change le nb de points de vie.
//**********************************************************
void HUDLife::SetLife( s32 life )
{
	m_Life = MATH_Clamp( life, (s32)0, m_MaxLife );
	m_LifeBar->SetWidth( GetBarWidth() );
}

//**********************************************************
// Donne le nb de points de vie.
//**********************************************************
u32 HUDLife::GetLife() const
{
	return m_Life;
}

//**********************************************************
// Calcule la largeur de la barre correspondante.
//**********************************************************
u32 HUDLife::GetBarWidth() const
{
	if( m_MaxLife <= 0 || m_Life <= 0 )
		return 0;
	
	// +0.999 pour qu'il y ai quand même 1 px tant qu'on a pas 0 pts de vie
	return (u32)((((float)m_Life/m_MaxLife)*LIFE_BAR_MAX_WIDHT)+0.999f);
}
