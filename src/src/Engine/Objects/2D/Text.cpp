#include	"Text.h"

#include	<assert.h>
#include	"Renderer/Renderer.h"

//******************************************************************

#define		TEXT_DEFAULT_COLOR			(Color4f( 0.f, 0.f, 0.f, 1.f ))
#define		TEXT_DEFAULT_FONT			"Arial"
#define		TEXT_DEFAULT_HEIGHT			12
#define		TEXT_DEFAULT_WEIGHT			FW_NORMAL

//**********************************************************
// Constructeur.
// @param[in]	text : Texte
//**********************************************************
Text::Text( const std::string &text )
: SceneObject2D()
{
	m_Text	= text;
	m_Font	= NULL;
	m_Color	= TEXT_DEFAULT_COLOR;

	// Desc par défaut
	sprintf( m_Desc.FaceName, TEXT_DEFAULT_FONT );
	m_Desc.Height			= TEXT_DEFAULT_HEIGHT;
	m_Desc.Width			= 0;
	m_Desc.Weight			= TEXT_DEFAULT_WEIGHT;
	m_Desc.MipLevels		= 0;
	m_Desc.Italic			= false;
	m_Desc.CharSet			= DEFAULT_CHARSET;
	m_Desc.OutputPrecision	= OUT_DEFAULT_PRECIS;
	m_Desc.Quality			= DEFAULT_QUALITY;
	m_Desc.PitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;
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
	RECT rct;
	rct.left=2;
	rct.right=300;
	rct.top=10;
	rct.bottom=rct.top+30;

	m_Font->DrawText(
		NULL,
		m_Text.c_str(),
		-1,
		&rct,
		DT_LEFT,
		D3DCOLOR_COLOR4F( m_Color )
	);
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
// Change la fonte (par défaut : Arial).
// @param[in]	fontName : Nom de la fonte
//**********************************************************
void Text::SetFont( const std::string &fontName )
{
	assert( fontName.size() < LF_FACESIZE );
	sprintf( m_Desc.FaceName, fontName.c_str() );
}

//**********************************************************
// Change la taille (hauteur) de la fonte.
// @param[in]	height : Hauteur (en px ?)
//**********************************************************
void Text::SetFontHeight( u32 height )
{
	m_Desc.Height = (int)height;
}

//**********************************************************
// Change l'épaisseur de la fonte.
// @param[in]	weight : Epaisseur (0->1000)
//**********************************************************
void Text::SetFontWeight( u32 weight )
{
	m_Desc.Weight = weight;
}

//**********************************************************
// Callback appelée juste après la création du device.
//**********************************************************
void Text::OnCreateDevice()
{
	SceneObject2D::OnCreateDevice();
	
	if( !m_Font )
	{
		D3DXCreateFontIndirect(
			Renderer::GetInstance()->m_pd3dDevice,
			&m_Desc,
			&m_Font
		);
	}
}

//**********************************************************
// Callback appelée juste après le reset du device.
//**********************************************************
void Text::OnResetDevice()
{
	SceneObject2D::OnResetDevice();
	
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
	SceneObject2D::OnLostDevice();
	
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
	SceneObject2D::OnDestroyDevice();
	
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
	return SceneObject2D::IsDrawable()
		&& m_Font;
}
