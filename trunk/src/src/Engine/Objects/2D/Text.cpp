#include	"Text.h"

#include	<assert.h>
#include	"Renderer/Renderer.h"

//******************************************************************

#define		TEXT_DEFAULT_FORMAT			(DT_TOP|DT_LEFT)
#define		TEXT_DEFAULT_COLOR			(Color4f( 0.f, 0.f, 0.f, 1.f ))
#define		TEXT_DEFAULT_FONT			"Arial"
#define		TEXT_DEFAULT_SIZE			12

//******************************************************************

//**********************************************************
// Initialisation commune aux constructeurs.
//**********************************************************
void Text::CommonInit()
{
	m_Format		= TEXT_DEFAULT_FORMAT;
	m_Color			= TEXT_DEFAULT_COLOR;
	m_Font			= NULL;

	m_BubbleWidth	= 0;
	m_BubbleHeight	= 0;
	m_Bubble.left	= 0;
	m_Bubble.top	= 0;
	m_Bubble.right	= 0;
	m_Bubble.bottom	= 0;

	// Initialisation de la bulle
	ComputeBubbleSize();
}

//**********************************************************
// Constructeur (fonte par défaut).
// @param[in]	text : Texte
//**********************************************************
Text::Text( const std::string &text )
: SceneObject2D()
{
	m_Text = text;
	CommonInit();

	// Propriétés de la fonte par défaut
	m_FontProperties.fontName	= TEXT_DEFAULT_FONT;
	m_FontProperties.size		= TEXT_DEFAULT_SIZE;
	m_FontProperties.bold		= false;
	m_FontProperties.italic		= false;
}

//**********************************************************
// Constructeur avec paramètres.
// @param[in]	text		: Texte
// @param[in]	fontName	: Nom de la fonte
// @param[in]	size		: Taille (px)
// @param[in]	bold		: Gras
// @param[in]	italic		: Italique
//**********************************************************
Text::Text( const std::string &text, const std::string &fontName, u32 size, bool bold, bool italic )
{
	m_Text = text;
	CommonInit();

	// Propriétés de la fonte
	m_FontProperties.fontName	= fontName;
	m_FontProperties.size		= size;
	m_FontProperties.bold		= bold;
	m_FontProperties.italic		= italic;
}

//**********************************************************
// Destructeur.
//**********************************************************
Text::~Text()
{
	OnLostDevice();
	OnDestroyDevice();
}

//**********************************************************
// Affiche l'objet.
//**********************************************************
void Text::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;
	pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
	pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
	
	// Rendu..
	m_Font->DrawText(
		NULL,
		m_Text.c_str(),
		-1,
		&m_Bubble,
		m_Format,
		D3DCOLOR_COLOR4F( m_Color )
	);

	pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
}

//**********************************************************
// Change le texte.
// @param[in]	text : Texte
//**********************************************************
void Text::SetText( const std::string &text )
{
	m_Text = text;
}

//**********************************************************
// Ajoute le texte à la suite.
// @param[in]	text : Texte à ajouter
//**********************************************************
void Text::AddText( const std::string &text )
{
	m_Text += text;
}

//**********************************************************
// Change le formatage du texte. Cf. winuser.h.
// Par défaut : DT_TOP|DT_LEFT.
// @param[in]	format : Flags de formatage
//**********************************************************
void Text::SetFormat( u32 format )
{
	m_Format = format;
}

//**********************************************************
// Change la taille de la bulle d'affichage.
// @param[in]	width	: Largeur en px
// @param[in]	height	: Hauteur en px
//**********************************************************
void Text::SetBubbleSize( u32 width, u32 height )
{
	if( width != m_BubbleWidth || height != m_BubbleHeight )
	{
		m_BubbleWidth = width;
		m_BubbleHeight = height;
		activate_dirty();
	}
}

//**********************************************************
// Change la taille de la bulle d'affichage.
// @param[in]	size : { largeur, hauteur }
//**********************************************************
void Text::SetBubbleSize( const Vector2i &size )
{
	SetBubbleSize( size.x, size.y );
}

//**********************************************************
// Change la largeur de la bulle d'affichage.
// @param[in]	width : Largeur en px
//**********************************************************
void Text::SetBubbleWidth( u32 width )
{
	SetBubbleSize( width, m_BubbleHeight );
}

//**********************************************************
// Change la hauteur de la bulle d'affichage.
// @param[in]	height : Hauteur en px
//**********************************************************
void Text::SetBubbleHeight( u32 height )
{
	SetBubbleSize( m_BubbleWidth, height );
}

//**********************************************************
// Calcule la taille de la bulle d'affichage en fonction du
// texte. Cette méthode utilise GDI et n'est pas très précise..
//**********************************************************
void Text::ComputeBubbleSize()
{
	SIZE textSize;
	HDC hDC = CreateCompatibleDC( NULL );
	GetTextExtentPoint32( hDC, m_Text.c_str(), m_Text.size(), &textSize );
	m_BubbleWidth	= textSize.cx;
	m_BubbleHeight	= textSize.cy;
	DeleteDC( hDC );
	activate_dirty();
}

//**********************************************************
// Vérifie si le point est dans la bulle d'affichage
// du texte.
// @param[in]	posX : Coordonnée x (px)
// @param[in]	posY : Coordonnée y (px)
// @return	Vrai si le point est en collision
//**********************************************************
bool Text::IsCollide( s32 posX, s32 posY )
{
	return posX >= m_Bubble.left
		&& posX <= m_Bubble.right
		&& posY >= m_Bubble.top
		&& posY <= m_Bubble.bottom;
}

//**********************************************************
// Callback appelée juste après la création du device.
//**********************************************************
void Text::OnCreateDevice()
{
	if( !m_Font )
	{
		D3DXCreateFont(
			Renderer::GetInstance()->m_pd3dDevice,
			(int)m_FontProperties.size,
			0,
			( m_FontProperties.bold ) ? FW_BOLD : FW_NORMAL,
			0,
			m_FontProperties.italic,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			m_FontProperties.fontName.c_str(),
			&m_Font
		);
	}
}

//**********************************************************
// Callback appelée juste après le reset du device.
//**********************************************************
void Text::OnResetDevice()
{
	if( m_Font )
	{
		m_Font->OnResetDevice();
	}
}

//**********************************************************
// Callback appelée à chaque fois qu'on perd le device.
//**********************************************************
void Text::OnLostDevice()
{
	if( m_Font )
	{
		m_Font->OnLostDevice();
	}
}

//**********************************************************
// Callback appelée à chaque fois qu'on détruit le device.
//**********************************************************
void Text::OnDestroyDevice()
{
	if( m_Font )
	{
		m_Font->Release();
		m_Font = NULL;
	}
}

//**********************************************************
// Vérifie que tout est pret pour l'affichage.
//**********************************************************
bool Text::IsDrawable() const
{
	return m_Font != NULL;
}

//**********************************************************
// Calcule le rectangle d'affichage.
//**********************************************************
void Text::dirty_Refresh()
{
	// On calcule la bulle d'affichage..
	m_Bubble.left	= (s32)m_Position.x - m_HotPoint.x;
	m_Bubble.top	= (s32)m_Position.y - m_HotPoint.y;
	m_Bubble.right	= m_Bubble.left + m_BubbleWidth;
	m_Bubble.bottom	= m_Bubble.top + m_BubbleHeight;
}
