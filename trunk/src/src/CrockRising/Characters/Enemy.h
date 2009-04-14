#ifndef		_Enemy_H_
#define		_Enemy_H_

//******************************************************************

#include		<Objects/SceneObjectAnimated.h>

//******************************************************************

class Enemy 
{
public:
	Enemy();
	~Enemy();

	int			 getLife () const { return m_iLife; }
	void	     setLife ( const int life ) { m_iLife += life; }   // La variable life représentant un bonus ou malus 

	D3DXVECTOR3  getPosition () const { return m_position; }
	void		 setPosition ( const D3DXVECTOR3 position ) { m_position = position; }

	D3DXVECTOR3  getDirection () const { return m_direction; }
	void		 setDirection ( const D3DXVECTOR3 direction ) { m_direction = direction; }

protected:
	int			 m_iLife;
	D3DXVECTOR3  m_position;
	D3DXVECTOR3  m_direction;


	SceneObjectAnimated*  m_pAnimated;

};

//******************************************************************
#endif


