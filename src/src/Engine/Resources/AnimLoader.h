#ifndef _ANIMLOADER_H_
#define _ANIMLOADER_H_

//******************************************************************

#include	<string>
#include    <cstring>  //pour la méthode c_str() (p-e include temporaire)
#include    <vector>
#include	<TinyXml/tinyxml.h>
#include	"Core/Types/Vector.h"

#include	"Resource.h"

using namespace std;

//******************************************************************

struct Bone {
	int iIndice ; 
	string sName ; 
	float **fMatrix ; 
	bool bBoneIsTransform ;
	vector<Bone> Son ; //Tableau contenant un ou plusieurs fils
};


//******************************************************************

class AnimLoader 
{
protected : 
	int m_iNbBones ;
	int m_iKeyFrame ; 
	float *m_fFrameValues ;
	float ****m_bonesMatrices ;
	string *m_sBonesName ;
	Bone m_rootBone ; 

public : 
	AnimLoader () {}
	AnimLoader (const char* sAnimPath);
	virtual ~AnimLoader(); 

	ResourceResult		Load (const char* sAnimPath , float ****&fBonesMatrice, float *&fTimeValues); 
	ResourceResult      GetSomeInfos (const char* sAnimPath);
	ResourceResult		FillMatrices ( TiXmlNode *rootNode ); 
	ResourceResult		ExtractMatricesDatas ( TiXmlNode *sourceNode );
	ResourceResult		ConvertTextToArray			(const char* ArrayText, float** Array, int iCount, int iStride);				// Remplit un double tableau de float à l'aide d'un texte
	ResourceResult		ConvertTextToArray			(const char* ArrayText, float*  Array, int iCount);	
	ResourceResult		ExtractHierarchyBones ( TiXmlNode *rootNode );

	Bone			BrowseBonesHierarchy (TiXmlNode* boneNode); //Fonction récursive

	void			ConvertToNoneHirearchy (Bone curBone, int parentId);
};

//******************************************************************

#endif // _ANIMLOADER_H