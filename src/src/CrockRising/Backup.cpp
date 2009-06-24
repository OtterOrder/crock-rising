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
 * Lit la backup à partir du fichier.
 * @param[in]	fileName : Chemin vers le fichier
 **********************************************************/
void CRBackup::ReadFromFile( const std::string &fileName )
{
	//TODO
}

/***********************************************************
 * Change la résolution enregistrée.
 * @param[in]	resolution : Résolution
 **********************************************************/
void CRBackup::SetResolution( const Vector2i &resolution )
{
	m_Resolution = resolution;
}

/***********************************************************
 * Donne la résolution.
 * @return	La résolution enregistrée
 **********************************************************/
const Vector2i& CRBackup::GetResolution() const
{
	return m_Resolution;
}
