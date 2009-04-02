#include	"Text.h"

#include	"Renderer/Renderer.h"

//******************************************************************

#define		TEXT_DEFAULT_COLOR		(Color4f( 0.f, 0.f, 0.f, 1.f ))
#define		TEXT_DEFAULT_BG_COLOR	(Color4f( 1.f, 1.f, 1.f, 1.f ))

//**********************************************************
// Constructeur.
// @param[in]	text : texte
//**********************************************************
Text::Text( const char *text )
: Object2D()
{
	m_Text	= text;
	m_pFont	= NULL;
}

//**********************************************************
// Destructeur.
//**********************************************************
Text::~Text()
{
	//TODO
}

//**********************************************************
// Affiche l'objet.
//**********************************************************
void Text::Draw()
{
	if( !IsDxReady() )
	{
		// Si les donn�es directx ne sont pas initialis�es,
		// on le fait au premier affichage, �a �vite de le
		// faire dans les constructeurs..
		InitDxData();
	}

	/*RECT rct;
	rct.left=2;
	rct.right=300;
	rct.top=10;
	rct.bottom=rct.top+30;

	m_pFont->DrawText(
		NULL,
		m_Text.c_str(),
		-1,
		&rct,
		DT_RIGHT,
		D3DCOLOR_COLOR4F( TEXT_DEFAULT_COLOR )
	);*/
}

//**********************************************************
// Change le texte.
//**********************************************************
void Text::SetText( const char *text )
{
	m_Text = text;
}

//**********************************************************
// Initialise les donn�es Dx.
//**********************************************************
void Text::InitDxData()
{
	// Cr�ation de la fonte
	if( !m_pFont )
	{
		/*D3DXCreateFont(
			Renderer::GetInstance()->m_pd3dDevice,
			30,
			0,
			FW_NORMAL,
			0,
			false,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			TEXT( "Arial" ),
			&m_pFont
		);*/
	}
}

//**********************************************************
// Lib�re les donn�es Dx.
//**********************************************************
void Text::ClearDxData()
{
	if( m_pFont )
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//**********************************************************
// V�rifie si les donn�es Dx sont pr�tes.
//**********************************************************
bool Text::IsDxReady() const
{
	return m_pFont != NULL;
}
