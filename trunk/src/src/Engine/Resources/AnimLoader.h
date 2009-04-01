#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Anim.h"
#include	<TinyXml/tinyxml.h>


//===========================================================================//
// Classe de chargement d'une animation                                      //
//===========================================================================//
class AnimLoader 
{
public : 
	AnimLoader () {}
	virtual ~AnimLoader() {} 

	// Chargement de l'animation
	ResourceResult Load(const char* sAnimPath, std::list < Bone * > &Bones, D3DXMATRIX &BindShape, int * &ArrayOrder, int &Nbframes); 

	// Chargement des bones
	ResourceResult LoadBones(std::list < Bone * > &Bones, TiXmlElement *rootNode, Bone * parent);
	ResourceResult FillNodeBone(std::list < Bone * > &Bones, TiXmlElement *boneNode, Bone * ParentBone);

	// Chargement des bind matrices
	ResourceResult LoadBindMatrices(std::list < Bone * > &Bones, TiXmlNode *rootNode, D3DXMATRIX &BindShape, int * &ArrayOrder);

	// Chargement de l'animation
	ResourceResult LoadAnimation(std::list < Bone * > &Bones, TiXmlNode *rootNode);
	ResourceResult LoadBoneAnimation(std::list < Bone * > &Bones, TiXmlNode *rootNode, TiXmlElement* Node);

	ResourceResult FillEmptyAnimation(std::list < Bone * > &Bones);

private :
	int m_Nbframes;

};

void ConvertStringToFloatArray(const char * Array, float * FloatArray, int iCount);
D3DXMATRIX LoadMatrix(float * FloatArray, int offset);