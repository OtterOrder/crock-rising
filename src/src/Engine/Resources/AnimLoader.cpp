#include "AnimLoader.h"

#define KEY_FRAME 61 //temporaire. Définir key_frame comme 
#define STRIDE 16

#define MATRIX_SIZE		4


AnimLoader::AnimLoader() 
{
	m_iNbBones = 22 ; //fixe pour l'instant 
	
	m_bonesMatrices = new float***[m_iNbBones];
	for (int i = 0 ; i < m_iNbBones ; i++){
		m_bonesMatrices[i] = new float**[KEY_FRAME];	
		for (int j = 0 ; j < KEY_FRAME ; j++){
			m_bonesMatrices[i][j] = new float*[4];
			for (int k = 0 ; k < 4 ; k++)
				m_bonesMatrices[i][j][k] = new float[4];
		}
	}

	m_indiceBone = 0 ; 

	m_sBonesName = new string[m_iNbBones];
}

/***********************************************************/

AnimLoader::~AnimLoader()
{
	delete [] m_sBonesName;
}

/***********************************************************/

ResourceResult AnimLoader::Load ( const char* sAnimPath ) 
{
	TiXmlDocument animFile( sAnimPath );

	if (!animFile.LoadFile()) 
		return RES_FAILED ; 

	TiXmlNode *rootNode ; 
	rootNode = animFile.FirstChild("COLLADA"); // Get root node

	FillMatrices(rootNode);
	ExtractHierarchyBones(rootNode);
	
	return RES_SUCCEED ; 
}

/***********************************************************/

ResourceResult AnimLoader::FillMatrices (TiXmlNode *rootNode)
{
	TiXmlNode *node , *temp ;  
	int test = 0 ; 

	if (rootNode) {
		node = rootNode->FirstChild("library_animations");
		if (node) {
			node = node->FirstChild("animation");
			
			while (node){
				temp = node ; //sauvegarde du noeud
				node = node->FirstChild("source");
				if (node){
					node = node->NextSibling("source");
					if (node){
						node = node->FirstChild("float_array");
						int testCount = 0 ; 
						node->ToElement()->Attribute("count",&testCount);
						if (testCount == 976)
							ExtractMatricesDatas(node);
					}
				}
				node = temp ; 
				node = node->NextSibling("animation");
			}
		}
	}
	return RES_SUCCEED ;
}

/***********************************************************/

ResourceResult AnimLoader::ExtractMatricesDatas(TiXmlNode *sourceNode)
{
	TiXmlNode *node = sourceNode ; 
	float **temp ; 

	const char* ArrayText ; 
	ArrayText = node->ToElement()->GetText();

	temp = new float*[KEY_FRAME];	
	for (int i=0 ; i < KEY_FRAME ; i++)
		temp[i] = new float[16];
	

	ConvertTextToArray(ArrayText,temp,KEY_FRAME,STRIDE);

	int indice = 0 ; 
	for ( int i = 0 ; i < KEY_FRAME ; ++i )
	{
       for (int j = 0 ; j < 4 ; ++j )
	   {
		   for (int k = 0 ; k < 4 ; ++k )
		   {
			  m_bonesMatrices[m_indiceBone][i][j][k] = temp[i][indice];
			  indice++ ; 
		   } 
	  }
	   indice = 0 ;
	}
	
	for (int i = 0 ; i < KEY_FRAME ; ++i)
		delete [] temp[i];
	delete [] temp ; 

	m_indiceBone++ ;

	return RES_SUCCEED ; 
}


/***********************************************************/

ResourceResult AnimLoader::ExtractHierarchyBones(TiXmlNode *rootNode)
{
	TiXmlNode *node ;  

	Bone listBone;
	
	if (rootNode) {
		node = rootNode->FirstChild("library_visual_scenes");	
		if (node) {
			node = node->FirstChild("visual_scene");
			if (node) {
				node = node->FirstChild("node");
				if (node) {
					node = node->NextSibling("node"); 
					string typeValue = node->ToElement()->Attribute("type"); //Racine de l'arbre type="NODE"

					node = node->FirstChild("node");
					string tm = node->ToElement()->Attribute("name");

					listBone = BrowseBonesHierarchy(node);
				}
			}
		}
	}

	return RES_SUCCEED ; 
}

/********************************************************************************/

Bone AnimLoader::BrowseBonesHierarchy (TiXmlNode* boneNode)
{
	Bone myBone;
	TiXmlNode* boneRoot = boneNode;
	TiXmlNode* curBone = boneRoot;

	if (curBone)
	{
		//Récupération des infos 
		string sCurrentName = curBone->ToElement()->Attribute("name");
		myBone.sName = sCurrentName;
		const char* cVerif = curBone->FirstChild()->ToElement()->Attribute("sid");
		if ( !cVerif )
			myBone.bBoneIsTransform = false ; 
		else 
			myBone.bBoneIsTransform = true ; 
		string sid = curBone->ToElement()->Attribute("sid");
		int iLenghtSid = sid.length();
		sid = sid.substr(4,(iLenghtSid-4));
		int iIndiceBone = atoi(sid.c_str()) - 1;
		myBone.iIndice = iIndiceBone ;

		//On remplit également la donnée membre m_bonesName qui nous servira 
		//à initialiser correctement le tableau 4d m_bonesMatrices
		//m_bonesName[iIndiceBone] = sCurrentName;
		m_sBonesName[iIndiceBone] = sCurrentName ;		

		//Récupération de la matrice associée 
		float **matrice ; 
		matrice = new float*[4]; 
		for (int i = 0 ; i < 4 ; ++i)
			matrice[i] = new float[4];

		const char *verif2 = curBone->FirstChild()->Value();
		if (strcmp(verif2,"matrix")==0)
			ConvertTextToArray(curBone->FirstChild()->ToElement()->GetText(),matrice,4,4);

		myBone.fMatrix = matrice ; 


		// Sauvergarde des données du bone
		curBone = curBone->FirstChild("node");

		while (curBone)		// parcours des bones fils
		{
			myBone.Son.push_back(BrowseBonesHierarchy(curBone));
			curBone = curBone->NextSibling("node");
		}
	}

	return myBone;
}


/***********************************************************
* Copie les données d'une chaîne caractères provenant du fichier XML dans un tableau
* @param[in]	text	: données sous forme de caractères
*				Array	: tableau à remplir
* @return	le résultat du chargement
**********************************************************/
ResourceResult	 AnimLoader::ConvertTextToArray	(const char* ArrayText, float** &Array, int iCount, int iStride)
{
	int iStartData = 0;
	int iDataSize = 0;

	for (int i = 0 ; i < iCount ; i++)
	{
		for (int j = 0 ; j < iStride ; j++) // matrice 4x4
		{
			iDataSize = 0;
			while (ArrayText[iStartData+iDataSize] != ' ')
				iDataSize ++;

			char* cData = new char[iDataSize];

			for (int k=0 ; k < iDataSize ; k++)
				if(ArrayText[iStartData +k] == ',')	cData[k] = '.';
				else cData[k] = ArrayText[iStartData +k];

				Array[i][j] = (float)atof(cData);

				delete [] cData;

				iStartData += iDataSize+1;
		}
	}

	return RES_FAILED;
}


/***********************************************************/
void AnimLoader::ConvertToNoneHirearchy (Bone curBone, int parentId)
{
	float tmpMatrix [MATRIX_SIZE][MATRIX_SIZE];
	float res;

	for (int frame=0 ; frame < KEY_FRAME ; frame++)
	{
		//m_bonesMatrices[curBone.iIndice][frame] *= m_bonesMatrices[parentId][frame];
		// Multiplie les matrices de transformation du bone avec celles de ses parents
		for (int i=0 ; i < MATRIX_SIZE ; i++)
		{
			for (int j=0 ; j < MATRIX_SIZE ; j++)
			{
				res = 0;
				for (int k=0 ; k < MATRIX_SIZE ; k++)
					res += m_bonesMatrices[curBone.iIndice][frame][i][k]*m_bonesMatrices[parentId][frame][k][j];

				tmpMatrix[i][j] = res;
			}
		}

		memcpy(m_bonesMatrices[curBone.iIndice][frame], tmpMatrix, sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);
	}

	for (int son=0 ; son < (int)curBone.Son.size() ; son ++)
		ConvertToNoneHirearchy(curBone.Son[son], curBone.iIndice);
}