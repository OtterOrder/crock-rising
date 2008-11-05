#ifndef		_Singleton_H
#define		_Singleton_H

//******************************************************************

template<typename T> class Singleton
{
	public:

		// =========================================================
		// M�thodes publiques
		
		Singleton*	GetInstance		( void );

	protected:

		// =========================================================
		// Donn�es priv�es
		
		static Singleton	*m_Instance;

		// =========================================================
		// M�thodes priv�es
		
		Singleton					( void );				// Constructeur
		~Singleton					( void );				// Destructeur
};

//******************************************************************
#endif		// _Singleton_H
