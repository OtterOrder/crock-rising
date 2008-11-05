#ifndef		_Singleton_H
#define		_Singleton_H

//******************************************************************

template< typename T >
class Singleton
{
	public:

		// =========================================================
		// Méthodes publiques
		
		static T*		GetInstance	( void );		// Donne l'instance du singleton
		static void		Release		( void );		// Détruit l'instance


	protected:

		// =========================================================
		// Méthodes protégées
		
		Singleton					( void ) {}		// Constructeur
		~Singleton					( void ) {}		// Destructeur


	private:

		// =========================================================
		// Données privées
		
		static T	*m_Instance;		// Instance du singleton

};

//******************************************************************
#endif		// _Singleton_H
