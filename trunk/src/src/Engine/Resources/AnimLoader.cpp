#include "AnimLoader.h"

#define STRIDE 16
#define MATRIX_SIZE 4

/***********************************************************/

AnimLoader::AnimLoader(const char* sAnimPath)
{
	GetSomeInfos(sAnimPath);

	m_bonesMatrices = new float***[m_iNbBones];
	for (int i = 0 ; i < m_iNbBones ; ++i){
		m_bonesMatrices[i] = new float**[m_iKeyFrame];	
		for (int j = 0 ; j < m_iKeyFrame ; ++j){
			m_bonesMatrices[i][j] = new float*[4];
			for (int k = 0 ; k < 4 ; ++k)
				m_bonesMatrices[i][j][k] = new float[4];
		}
	} 

	m_fBindShapeMatrix = new float*[4];
	for (int i = 0 ; i < 4 ; i++)
		m_fBindShapeMatrix[i] = new float[4];

	m_fBindPosesArray = new float**[m_iNbBones];
	for (int i = 0 ; i < m_iNbBones ; i++)
	{
		m_fBindPosesArray[i] = new float*[4];
		for (int j = 0 ; j < 4 ; j++)
			m_fBindPosesArray[i][j] = new float[4];
	}

	m_sBonesName = new string[m_iNbBones];

	m_fFrameValues = new float[m_iKeyFrame];
}

/***********************************************************/

AnimLoader::~AnimLoader()
{
	delete [] m_sBonesName ;
	delete [] m_fFrameValues ;

	//Destruction tableau 4d
	for (int i = 0 ; i < m_iNbBones ; ++i)
	{
		for (int j = 0 ; j < m_iKeyFrame ; ++j)
		{
			for (int k = 0 ; k < 4 ; ++k)
				delete [] m_bonesMatrices[i][j][k];

			delete [] m_bonesMatrices[i][j];
		}
		delete [] m_bonesMatrices[i];
	}
	delete [] m_bonesMatrices ;

	


	for (int i = 0 ; i < 4 ; i++)
		delete[] m_fBindShapeMatrix[i];

	delete [] m_fBindShapeMatrix;


	for (int i = 0 ; i < m_iNbBones ; ++i)
	{
		for (int j = 0 ; j < 4 ; ++j)
		{
			delete [] m_fBindPosesArray[i][j];
		}
		delete [] m_fBindPosesArray[i];
	}
	delete [] m_fBindPosesArray ;



}

/***********************************************************/

ResourceResult AnimLoader::Load ( const char* sAnimPath , float ****&fBonesMatrice, float *&fTimeValues, float **&fBindShapeMatrix, float ***&fBindPosesArray ) 
{
	TiXmlDocument animFile( sAnimPath );

	if (!animFile.LoadFile()) 
		return RES_FAILED ; 

	TiXmlNode *rootNode ; 
	rootNode = animFile.FirstChild("COLLADA"); // Get root node

	ExtractHierarchyBones(rootNode);
	FillMatrices(rootNode);

	//Allocation puis remplissage des matrices que l'on récupère depuis une instance de la classe Anim

	//fBonesMatrice 
	fBonesMatrice = new float***[m_iNbBones];
	for (int i = 0 ; i < m_iNbBones ; ++i){
		fBonesMatrice[i] = new float**[m_iKeyFrame];	
		for (int j = 0 ; j < m_iKeyFrame ; ++j){
			fBonesMatrice[i][j] = new float*[4];
			for (int k = 0 ; k < 4 ; ++k)
				fBonesMatrice[i][j][k] = new float[4];
		}
	} 



	for (int i = 0 ; i < m_iNbBones ; i++)
	{
		for (int j = 0 ; j < m_iKeyFrame ; j++)
		{
			for (int k = 0 ; k < 4 ; k++) 
			{
				for (int l = 0 ; l < 4 ; l++)
					fBonesMatrice[i][j][k][l] = m_bonesMatrices[i][j][k][l];
			}
		}
	}


	//fTimeValues
	fTimeValues = new float[m_iKeyFrame];

	for (int i = 0 ; i < m_iKeyFrame ; i++)
		fTimeValues[i] = m_fFrameValues[i];


	//fBindShapeMatrix
	fBindShapeMatrix = new float*[4];
	for (int i = 0 ; i < 4 ; i++)
		fBindShapeMatrix[i] = new float[4];

	for(int i = 0 ; i < 4 ; i++)
		for (int j = 0 ; j < 4 ; j++)
			fBindShapeMatrix[i][j] = m_fBindShapeMatrix[i][j];


	//fBindPosesArray
	fBindPosesArray = new float**[m_iNbBones];
	for (int i = 0 ; i < m_iNbBones ; i++)
	{
		fBindPosesArray[i] = new float*[4];
		for (int j = 0 ; j < 4 ; j++)
			fBindPosesArray[i][j] = new float[4];
	}

	for (int i = 0 ; i < m_iNbBones ; i++)
	{
		for (int j = 0 ; j < 4 ; j++)
		{
			for (int k = 0 ; k < 4 ; k++)
				fBindPosesArray[i][j][k] = m_fBindPosesArray[i][j][k];
		}
	}

	return RES_SUCCEED ; 
}

/***********************************************************/

ResourceResult AnimLoader::GetSomeInfos( const char* sAnimPath )
{
	TiXmlDocument animFile( sAnimPath );

	if (!animFile.LoadFile()) 
		return RES_FAILED ; 
	
	TiXmlNode *node, *rootNode ; 
	rootNode = animFile.FirstChild("COLLADA"); // Get root node
	node = rootNode ; 

	//Dans un premier temps, récupération du nombre de bones
	if (node)
	{
		node = node->FirstChild("library_controllers");
		if (node)
		{
			node = node->FirstChild("controller");
			if(node)
			{
				node = node->FirstChild("skin");
				if(node)
				{
					node = node->FirstChild("source");
					if(node)
					{
					    node->FirstChild("Name_array")->ToElement()->Attribute("count",&m_iNbBones);
					}
				}
			}
		}

	}

	//Ensuite, on récupère le nombre de KeyFrame
	node = rootNode ; 
	if (node)
	{
		node = node->FirstChild("library_animations");
		if (node)
		{
			node = node->FirstChild("animation");
			if(node)
			{
				node = node->FirstChild("source");
				if(node)
				{
					node->FirstChild("float_array")->ToElement()->Attribute("count",&m_iKeyFrame);
				}
			}
		}
		
	}
	return RES_SUCCEED ; 
}

/***********************************************************/

ResourceResult AnimLoader::FillMatrices (TiXmlNode *rootNode)
{
	TiXmlNode *node , *temp ;  
	int test = 0 ; 
	bool fillArrayKeyFrameValues = false ; 

	if (rootNode) {
		node = rootNode->FirstChild("library_animations");
		if (node) {
			node = node->FirstChild("animation");

			while (node){
				temp = node ; //sauvegarde du noeud
				node = node->FirstChild("source");
				if(!fillArrayKeyFrameValues)
				{
					node = node->FirstChild("float_array");
					const char* ArrayText ; 
					ArrayText = node->ToElement()->GetText();
					ConvertTextToArray(ArrayText,m_fFrameValues,m_iKeyFrame);
					fillArrayKeyFrameValues = true ; 
					node = temp ; 
				}

				if (node){
					node = node->NextSibling("source");
					if (node){
						node = node->FirstChild("float_array");
							ExtractMatricesDatas(node);
					}
				}
				node = temp ; 
				node = node->NextSibling("animation");
			}
		}
	}

	//Pour les matrices de bones qui ne subissent pas de transformations, on les remplit
	//avec des matrices identitées
	for (int i = 0 ; i < m_iNbBones ; ++i )
	{
		//Test la 1ère valeur pour savoir si la matrice a été remplie ou non
		if (  m_bonesMatrices[i][0][0][0] < -400000000.f ) //matrice non remplie  -4.3160208e+008
		{
			for (int j = 0 ; j < m_iKeyFrame ; ++j)
			{
				for (int k = 0 ; k < 4 ; ++k)
				{
					for (int l = 0 ; l < 4 ; ++l)
					{
						if(k==l)					
							m_bonesMatrices[i][j][k][l] = 1.f ;
						else 
							m_bonesMatrices[i][j][k][l] = 0.f ;

					}
				}
			}
		}

	}

	for (int i = 0 ; i < (int)m_rootBone.Son.size() ; i++ )
		;//ConvertToNoneHirearchy(m_rootBone.Son[i],m_rootBone.iIndice);


	//Récupération de BindShapeMatrix et bindPosesArray
	float *tempArray = new float[m_iNbBones*STRIDE]; 
	node = rootNode->FirstChild("library_controllers");
	if (node)
	{
			node = node->FirstChild("controller");
			if(node)
			{
				node = node->FirstChild("skin");
				temp = node ;
				if(node)
				{
					node = node->FirstChild("bind_shape_matrix");
					if(node)
					{
						const char* ArrayText ; 
						ArrayText = node->ToElement()->GetText();
						ConvertTextToArray(ArrayText,m_fBindShapeMatrix,4,4);

						node = temp ; 
						node = node->FirstChild("source");
						if (node)
						{
							node = node->NextSibling("source");
							if (node)
							{
								node = node->FirstChild("float_array"); 
								if (node)
								{ 
									int iCount = m_iNbBones*STRIDE ; 
									ArrayText = node->ToElement()->GetText();
									ConvertTextToArray(ArrayText,tempArray,iCount);
									int indice = 0 ; 
									for (int i = 0 ; i < m_iNbBones ; i++)
									{
										for (int j = 0 ; j < 4 ; j++)
										{
											for (int k = 0 ; k < 4 ; k++){
												m_fBindPosesArray[i][j][k] = tempArray[indice];
												indice++ ; 
											}
										}
									}
								}
								}
							}
						}
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

	temp = new float*[m_iKeyFrame];	
	for (int i=0 ; i < m_iKeyFrame ; i++)
		temp[i] = new float[16];


	ConvertTextToArray(ArrayText,temp,m_iKeyFrame,STRIDE);

	string sGetNameBone = node->ToElement()->Attribute("id");
	int iDashPos = sGetNameBone.find_first_of('-') ; //On récupère la position du tiret
	sGetNameBone = sGetNameBone.substr(0,iDashPos);

	int ind = 0 ;
	while ( ind < m_iNbBones )
	{
		if ( strcmp (m_sBonesName[ind].c_str(),sGetNameBone.c_str()) == 0 )
			break ; 
		++ind ; 
	}

	int indice = 0 ; 
	for ( int i = 0 ; i < m_iKeyFrame ; ++i )
	{
		for (int j = 0 ; j < 4 ; ++j )
		{
			for (int k = 0 ; k < 4 ; ++k )
			{
				m_bonesMatrices[ind][i][j][k] = temp[i][indice];
				indice++ ; 
			} 
		}
		indice = 0 ;
	}

	for (int i = 0 ; i < m_iKeyFrame ; ++i)
		delete [] temp[i];
	delete [] temp ; 

	return RES_SUCCEED ; 
}


/***********************************************************/

ResourceResult AnimLoader::ExtractHierarchyBones(TiXmlNode *rootNode)
{
	TiXmlNode *node ;  

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

					m_rootBone = BrowseBonesHierarchy(node);
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

		for (int i = 0 ; i < 4 ; ++i)
			delete [] matrice[i]; 
		delete [] matrice ; 



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


//===========================================================================//
// Copie les données d'une chaîne caractères provenant du fichier XML dans   //
// un tableau.																 //
//===========================================================================//
ResourceResult AnimLoader::ConvertTextToArray(const char* ArrayText, float** Array, int iCount, int iStride)
{
	float* tmpArray = new float[iCount*iStride];
	ConvertTextToArray(ArrayText, tmpArray, iCount*iStride);

	for (int i = 0 ; i < iCount ; i++)
		for (int j = 0 ; j < iStride ; j++)
			Array[i][j] = tmpArray[i*iStride + j];

	delete [] tmpArray;

	return RES_SUCCEED;
}

//===========================================================================//

ResourceResult	 AnimLoader::ConvertTextToArray (const char* ArrayText, float*  Array, int iCount)
{
	int id=0;
	int dec=0;

	float result	= 0;
	float signe		= 1;
	float mantisse	= 0.1f;

	for (int i = 0 ; i < iCount ; i++)
	{
		while(ArrayText[id+dec] != ' ' && ArrayText[id+dec] != '\0')
		{			
			if(ArrayText[id]!= '.')
			{
				if(ArrayText[id]== '-')
					signe=-1;	
				else
					result=ArrayText[id]-'0'+result*10;	
				id++;
			}
			else
			{	
				dec++;	
				result=((ArrayText[id+dec]-'0')*mantisse)+result;
				mantisse *= 0.1f;

				if(ArrayText[id+dec+1] == ' ')
					dec++;

			}
		}
		Array[i]=result*signe;
		id=id+dec+1;
		dec=0, result=0, mantisse=0.1f, signe=1;
	}

	return RES_SUCCEED;
}
/*
ResourceResult ConvertTextToMatrix (const char* ArrayText, float** Matrix, int iSize)
{
	float* tmpMatrix = new float[iSize*iSize];
	ConvertTextToArray(ArrayText, tmpMatrix, iSize*iSize);

	for (int i=0 ; i < iSize ; i++)
		for (int j=0 ; j < iSize ; j++)
			Matrix[i][j] = 
}
*/
/**************************************************************************/

void AnimLoader::ConvertToNoneHirearchy (Bone curBone, int parentId)
{
	float tmpMatrix [MATRIX_SIZE][MATRIX_SIZE];
	float res;

	for (int frame=0 ; frame < m_iKeyFrame ; frame++)
	{
		// Multiplie les matrices de transformation du bone avec celles de ses parents
		/*
		for (int i=0 ; i < 4 ; i++)
		{
			for (int j=0 ; j < 4 ; j++)
			{
				res = 0;
				for (int k=0 ; k < 4 ; k++)
					res += m_bonesMatrices[curBone.iIndice][frame][i][k]*m_bonesMatrices[parentId][frame][k][j];

				tmpMatrix[i][j] = res;
			}
		}

		for (int i=0 ; i < 4 ; i++)
			for (int j=0 ; j < 4 ; j++)
				m_bonesMatrices[curBone.iIndice][frame][i][j] = tmpMatrix[i][j];
		//*/


		D3DXMATRIX curSkinTransform;
		for (int i=0 ; i<4 ; i++)
				for (int j=0 ; j<4 ; j++)
					curSkinTransform(i,j) = m_bonesMatrices[curBone.iIndice][frame][i][j];

		D3DXMATRIX parentSkinTransform;
		for (int i=0 ; i<4 ; i++)
				for (int j=0 ; j<4 ; j++)
					parentSkinTransform(i,j) = m_bonesMatrices[parentId][frame][i][j];

		D3DXMatrixMultiply(&curSkinTransform, &parentSkinTransform, &curSkinTransform);

		for (int i=0 ; i < 4 ; i++)
			for (int j=0 ; j < 4 ; j++)
				m_bonesMatrices[curBone.iIndice][frame][i][j] = curSkinTransform(i,j);
	}

	for (int son=0 ; son < (int)curBone.Son.size() ; son ++)
		ConvertToNoneHirearchy(curBone.Son[son], curBone.iIndice);
}
