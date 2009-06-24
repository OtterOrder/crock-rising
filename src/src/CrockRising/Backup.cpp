#include	"Backup.h"

//******************************************************************

#define		DEFAULT_RESOLUTION		Vector2i( 800, 600 )

//******************************************************************

/***********************************************************
 * Constructeur.
 **********************************************************/
CRBackup::CRBackup( void )
{
	m_Resolution = DEFAULT_RESOLUTION;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
CRBackup::~CRBackup( void )
{
	//TODO
}

/***********************************************************
 * Lit la backup � partir du fichier.
 * @param[in]	fileName : Chemin vers le fichier
 **********************************************************/
void CRBackup::ReadFromFile( const std::string &fileName )
{
	//TODO
}

/***********************************************************
 * Change la r�solution enregistr�e.
 * @param[in]	resolution : R�solution
 **********************************************************/
void CRBackup::SetResolution( const Vector2i &resolution )
{
	m_Resolution = resolution;
}

/***********************************************************
 * Donne la r�solution.
 * @return	La r�solution enregistr�e
 **********************************************************/
const Vector2i& CRBackup::GetResolution() const
{
	return m_Resolution;
}
