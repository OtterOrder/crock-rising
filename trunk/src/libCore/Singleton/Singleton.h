#ifndef		_Singleton_H
#define		_Singleton_H

//******************************************************************

template< typename T >
class Singleton
{
	public:

		// =========================================================
		// M�thodes publiques
		
		static T*		GetInstance	( void );		// Donne l'instance du singleton
		static void		Release		( void );		// D�truit l'instance


	protected:

		// =========================================================
		// M�thodes prot�g�es
		
		Singleton					( void ) {}		// Constructeur
		~Singleton					( void ) {}		// Destructeur


	private:

		// =========================================================
		// Donn�es priv�es
		
		static T	*m_Instance;		// Instance du singleton

};

//******************************************************************
#endif		// _Singleton_H
