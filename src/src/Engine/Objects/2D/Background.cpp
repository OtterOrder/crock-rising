#include	"Background.h"

#include	"Renderer/Renderer.h"

//******************************************************************

#define		BG_DEFAULT_PRIORITY		(255)

//**********************************************************
// Constructeur.
// @param[in]	path : chemin vers l'image (texture)
//**********************************************************
Background::Background( const char *path )
: Sprite( path )
{
	// Le background est derrière tous les sprites
	m_Priority = BG_DEFAULT_PRIORITY;
}

//**********************************************************
// Destructeur.
//**********************************************************
Background::~Background()
{
	//TODO
}

//**********************************************************
// Initialise les données Dx.
//**********************************************************
void Background::InitDxData()
{
	Sprite::InitDxData();

	// La taille du sprite correspond à la taille de la fenêtre
	m_Width = Renderer::GetInstance()->GetWindowWidth();
	m_Height = Renderer::GetInstance()->GetWindowHeight();
}
