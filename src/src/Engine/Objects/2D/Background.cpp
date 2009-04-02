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
	// Le background est derri�re tous les sprites
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
// Initialise les donn�es Dx.
//**********************************************************
void Background::InitDxData()
{
	Sprite::InitDxData();

	// La taille du sprite correspond � la taille de la fen�tre
	m_Width = Renderer::GetInstance()->GetWindowWidth();
	m_Height = Renderer::GetInstance()->GetWindowHeight();
}
