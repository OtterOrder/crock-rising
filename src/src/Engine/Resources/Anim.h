#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Resource.h"
#include <list>


//===========================================================================//
// Classe comprenant toutes les données utiles pour un bone                  //
//===========================================================================//
class Bone
{
public:
	Bone() {}
	Bone(D3DXMATRIX setMatrix, Bone * ParentBone, int setNum, std::string setSid);
	~Bone();
	
	int							Num;					// Numéro du bone
	std::string					Sid;					// String pour le nom du bone
	std::string					id;						// String pour l'id du bone
	D3DXVECTOR3					Position;				// Position du bone
	D3DXMATRIX					InitialMatrix;			// Matrice initiale du bone
	Bone*						Parent;					// Bone parent
	std::list < Bone* >			Children;				// Liste des enfants
	D3DXMATRIX*					animationMatrix;		// Liste de matrice pour l'animation du bone
	D3DXMATRIX					invBoneSkinMatrix;		// Matrice pour passer dans le repère local du bone
	D3DXMATRIX					FinalMatrix;			// Matrice finale envoyée au shader

	D3DXMATRIX GetMatrixRecursively();

};

//===========================================================================//
// Classe de la ressource Animation                                          //
//===========================================================================//
class Anim : public Resource 
{

public : 
	Anim () {}
	virtual ~Anim();

	ResourceResult Load ( std::string resource , ResourceParam param);
	void CalculateAbsoluteBoneMatrices();

	std::list < Bone * >  m_Bones;				// Liste des bones de l'animation
	D3DXMATRIX			  m_BindShape;			// Bind shape matrix	
	int*				  m_BoneArrayOrder;		// Tableau des identifiants des bones
	int					  m_NbFrames;			// Nombre de frame de l'animation

};


