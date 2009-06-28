#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Resource.h"
#include <vector>



//===========================================================================//
// Classe comprenant toutes les donn�es utiles pour un bone                  //
//===========================================================================//
class Bone
{
public:
	Bone() {}
	Bone(D3DXMATRIX setMatrix, Bone * ParentBone, int setNum, std::string setSid);
	~Bone();
	
	int							Num;					// Num�ro du bone
	std::string					Sid;					// String pour le nom du bone
	std::string					id;						// String pour l'id du bone
	D3DXVECTOR3					Position;				// Position du bone
	D3DXMATRIX					InitialMatrix;			// Matrice initiale du bone
	Bone*						Parent;					// Bone parent
	std::vector < Bone* >			Children;				// Liste des enfants
	D3DXMATRIX*					animationMatrix;		// Liste de matrice pour l'animation du bone
	D3DXMATRIX					invBoneSkinMatrix;		// Matrice pour passer dans le rep�re local du bone
	D3DXMATRIX					FinalMatrix;			// Matrice finale envoy�e au shader

	D3DXMATRIX GetMatrixRecursively();

	// Surcharge de l'op�rateur <
	//friend bool operator < (const Bone & lhs, const Bone & rhs)
	//{
	//	return (lhs.Num < rhs.Num ? true : false);
	//}

};

struct BoneSort
{
	bool operator ()(Bone * a1, Bone * a2) const
	{
		if (a1 != NULL && a2 != NULL) 
			return a1->Num < a2->Num;
		else return false;
	}
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

	std::vector < Bone * >  m_Bones;				// Liste des bones de l'animation
	D3DXMATRIX			  m_BindShape;			// Bind shape matrix	
	int*				  m_BoneArrayOrder;		// Tableau des identifiants des bones
	int					  m_NbFrames;			// Nombre de frame de l'animation

};


