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
	float ****m_bonesMatrices ;
	int m_indiceBone ; // indice pour la matrice m_bonesMatrices , variable temporaire (?)
	string *m_sBonesName ;

public : 
	AnimLoader ();
	virtual ~AnimLoader(); 

	ResourceResult Load (const char* sAnimPath ); 
	ResourceResult FillMatrices ( TiXmlNode *rootNode ); 
	ResourceResult ExtractMatricesDatas ( TiXmlNode *sourceNode );
	ResourceResult ConvertTextToArray	( const char* ArrayText, float** &Array, int iCount, int iStride );
	ResourceResult ExtractHierarchyBones ( TiXmlNode *rootNode );

	Bone			BrowseBonesHierarchy (TiXmlNode* boneNode); //Fonction récursive

	void			ConvertToNoneHirearchy (Bone curBone, int parentId);
};

//******************************************************************

#endif // _ANIMLOADER_H