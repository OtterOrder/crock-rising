#ifndef _MACROS_H_
#define _MACROS_H_

//////////////////////////////////////////////////////////////////////////
// EXEMPLE : 
//	release(m_pCamera);
//////////////////////////////////////////////////////////////////////////
#define release( objet ) if(objet) delete objet; objet = NULL;

//////////////////////////////////////////////////////////////////////////
// EXEMPLE :
// while( !Enemy::RefList.empty() )
// {
// 		Enemy* enemy = Enemy::RefList.back() ;
//		releaseFromList( Enemy::RefList, enemy);
// }
//////////////////////////////////////////////////////////////////////////
#define releaseFromList( list, objet ) list.remove( objet) ; if(objet) delete objet; objet = NULL;

//////////////////////////////////////////////////////////////////////////
// EXEMPLE : 
// i = m_SoundMap[Perso::HIT].size()-1;
// while( !m_SoundMap[Perso::HIT].empty() )
// {
// 		releaseFromVector( m_SoundMap[Perso::HIT], i); --i;
// }
//////////////////////////////////////////////////////////////////////////
#define releaseFromVector( vector, offset ) if(vector[offset]) delete vector[offset]; vector[offset] = NULL; vector.erase( vector.begin() + offset ); 

//////////////////////////////////////////////////////////////////////////
// EXEMPLE : 
// if ( random( 0, 1 ))	enemy = new Alien( Vector3f(spawnX, 8.f, spawnZ) );
// else					enemy = new MmeGrise( Vector3f(spawnX, 8.f, spawnZ) );
//////////////////////////////////////////////////////////////////////////
#define random(min, max) rand() % (max-min+1) + min;


#define initRandom() srand( (unsigned)time(NULL) );

//#define TEST_ARME

#endif
