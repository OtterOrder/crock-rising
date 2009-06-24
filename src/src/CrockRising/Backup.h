#ifndef		_Backup_H_
#define		_Backup_H_

//******************************************************************

#include	<Core/Types/Vector.h>
#include	<Game/Game.h>

//******************************************************************

class CRBackup : public Game::Backup
{
public:

	CRBackup			( void );	// Constructeur
	virtual ~CRBackup	( void );	// Destructeur

	void ReadFromFile( const std::string &fileName );

	void SetResolution( const Vector2i &resolution );
	const Vector2i& GetResolution() const;

protected:

	Vector2i	m_Resolution;

};

//******************************************************************
#endif		//_Backup_H_
