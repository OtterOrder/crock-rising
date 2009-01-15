#include "MeshLoader.h"

//******************************************************************

/***********************************************************
* Constructeur.
**********************************************************/
MeshLoader::MeshLoader()
{

	m_iNbVertices	= 0;
	m_iNbFaces		= 0;

	m_Positions	= NULL;
	m_Normals	= NULL;
	m_TexCoords	= NULL;

	m_iNbPositions	= 0;
	m_iNbNormals	= 0;
	m_iNbTexCoords	= 0;


	m_Faces		= NULL;
}

/***********************************************************
* Destructeur.
**********************************************************/
MeshLoader::~MeshLoader()
{
	//*
	if (m_Positions)
	{
		for (int i = 0 ; i < m_iNbPositions ; i++)
			delete m_Positions[i];
		delete [] m_Positions;
	}

	if (m_Normals)
	{
		for (int i = 0 ; i < m_iNbNormals ; i++)
			delete m_Normals[i];
		delete [] m_Normals;
	}

	if (m_TexCoords)
	{
		for (int i = 0 ; i < m_iNbTexCoords ; i++)
			delete m_TexCoords[i];
		delete [] m_TexCoords;
	}

	if (m_Faces)
		delete [] m_Faces;
	//*/
}


/***********************************************************
* Supprime la ressource.
* @param[in]	resource : crc32 de la ressource
* @return	le résultat du chargement
**********************************************************/
ResourceResult MeshLoader::Load(const char *sMeshPath,  Vertex *&VertexBuffer, int *&IndexBuffer, int &iNbVertices, int &iNbIndex)
{

	TiXmlDocument meshFile( sMeshPath );
	if (!meshFile.LoadFile ())
		return RES_FAILED;

	TiXmlNode* rootNode;

	rootNode =  meshFile.FirstChild( "COLLADA" );  // Get Root Node

	FillArrays (rootNode, VertexBuffer, IndexBuffer);

	iNbVertices	= m_iNbVertices;
	iNbIndex	= m_iNbFaces*NbFaceVertices;

	return RES_SUCCEED;
}


/***********************************************************
* Rempli les tableaux de données.
* @param[in]	rootNode : balise racine du fichier XML
* @return	le résultat du chargement
**********************************************************/
ResourceResult	 MeshLoader::FillArrays	(TiXmlNode* rootNode,  Vertex *&VertexBuffer, int *&IndexBuffer)
{
	TiXmlNode* node;
	TiXmlNode* MeshNode;

	//const char * SourceId;

	/*string sPositionsId ("Position");
	string sNormalsId	("Normal0");
	string sTexCoordsId ("UV0");*/

	string sPositionsId ("positions");
	string sNormalsId	("normals");
	string sTexCoordsId ("channel1");



	if(rootNode) {
		node =  rootNode->FirstChild( "library_geometries" );
		if(node) {
			node =  node->FirstChild( "geometry" );
			if(node) {
				node =  node->FirstChild( "mesh" );
				MeshNode = node;
				if(node) {
					node =  node->FirstChild( "source" );		// Recherche la première balise "source"

					while (node)
					{
						string sId (node->ToElement()->Attribute("id"));		// vérification de l'id
						size_t SpliterPlace = sId.rfind("-")+1;

						if (SpliterPlace != sId.npos)
						{
							if (sId.compare( SpliterPlace, sPositionsId.length(), sPositionsId) == 0)
							{
								ExtractArrayDatas (node, m_Positions, m_iNbPositions);
							}
							else if (sId.compare( SpliterPlace, sNormalsId.length(), sNormalsId) == 0)
							{
								ExtractArrayDatas (node, m_Normals, m_iNbNormals);
							}
							else if (sId.compare( SpliterPlace, sTexCoordsId.length(), sTexCoordsId) == 0)
							{
								ExtractArrayDatas (node, m_TexCoords, m_iNbTexCoords);
							}
						}

						node = node->NextSibling( "source" );
					}

					node = MeshNode;

					node =  node->FirstChild( "triangles");
					if (node)
						FillVBArray (node, VertexBuffer, IndexBuffer);
				}
			}
		}
	}

	return RES_FAILED;
}

/***********************************************************
* Etrait les données lées à un tableau.
* @param[in]	sourceNode : balise source (source des données) du fichier XML
*				Array : tableau à remplir
* @return	le résultat du chargement
**********************************************************/
ResourceResult	 MeshLoader::ExtractArrayDatas	(TiXmlNode* sourceNode, float** &Array, int &iNbElements)
{
	TiXmlNode* node = sourceNode;			// Enregistrement de la balise source

	const char* ArrayText;

	int iNbVertices = 0;
	int iStride = 0;


	node =  node->FirstChild( "technique_common" );
	if(node) {
		node =  node->FirstChild( "accessor" );
		if(node) {

			node->ToElement()->Attribute("count", &iNbVertices);		// Stockage du nombre de vertices
			if (m_iNbVertices < iNbVertices)	m_iNbVertices = iNbVertices;	// Enregistrement du plus grand nombre de vertices

			iNbElements = iNbVertices;

			node->ToElement()->Attribute("stride", &iStride);			// Stockage de la taille du tableau				

			if (!Array && iNbVertices && iStride)			// Si le tableau n'a pas été alloué et que le nombre de vertices et la taille sont connus
			{
				Array = new float*[iNbVertices];				// Le tableau est alloué
				for (int i=0 ; i<iNbVertices ; i++)
					Array[i] = new float[iStride];
			}
		}
	}					

	node = sourceNode;
	node =  node->FirstChild( "float_array" );
	if(node) {
		ArrayText = node->ToElement()->GetText();

		ConvertTextToArray(ArrayText, Array, iNbVertices, iStride);

		//delete text;
	}

	return RES_FAILED;
}

/***********************************************************
* Copie les données d'une chaîne caractères provenant du fichier XML dans un tableau
* @param[in]	text	: données sous forme de caractères
*				Array	: tableau à remplir
*				iCount	: nombre de cases du tableau
*				iStride	: nombre de données par case
* @return	le résultat du chargement
**********************************************************/
ResourceResult	 MeshLoader::ConvertTextToArray	(const char* ArrayText, float** &Array, int iCount, int iStride)
{
	int iStartData = 0;
	int iDataSize = 0;

	for (int i = 0 ; i < iCount ; i++)
	{
		for (int j = 0 ; j < iStride ; j++)
		{
			iDataSize = 0;
			while (ArrayText[iStartData+iDataSize] != ' ')
				iDataSize ++;

			char* cData = new char[iDataSize];

			for (int k=0 ; k < iDataSize ; k++)
				if(ArrayText[iStartData +k] == ',')	cData[k] = '.';
				else							cData[k] = ArrayText[iStartData +k];

				Array[i][j] = (float)atof(cData);

				delete [] cData;

				iStartData += iDataSize+1;
		}
	}

	return RES_FAILED;
}

ResourceResult	 MeshLoader::FillVBArray	(TiXmlNode* TrianglesNode,  Vertex *&VertexBuffer, int *&IndexBuffer)
{
	TiXmlNode* node = TrianglesNode;			// Enregistrement de la balise triangles

	node->ToElement()->Attribute("count", &m_iNbFaces);

	FillFacesArray ( node );

	VertexBuffer = new Vertex [m_iNbVertices];
	IndexBuffer  = new int [m_iNbFaces*NbFaceVertices];

	int FaceIndex;

	for (int i = 0 ; i < m_iNbFaces*NbFaceVertices ; i ++)
	{
		FaceIndex = 0;
		while (FaceIndex < i && m_Faces[FaceIndex] != m_Faces[i] )
			FaceIndex ++;

		if ( FaceIndex < i )
		{
			IndexBuffer[i] = m_Faces[FaceIndex].m_Normal;
		}
		else
		{
			IndexBuffer[i] = m_Faces[i].m_Normal;
			FillVertex(m_Faces[i].m_Normal, i, VertexBuffer, IndexBuffer);
		}
	}

	return RES_SUCCEED;
}

void	 MeshLoader::FillVertex			(int VertexIndex, int FaceIndex,  Vertex *&VertexBuffer, int *&IndexBuffer)
{
	Vector3f VectPosition	(0.f, 0.f, 0.f);
	Vector3f VectNormal		(0.f, 0.f, 0.f);
	Vector2f VectTexCoord	(0.f, 0.f);


	if ( m_Positions )
	{
		float*  Position = m_Positions[m_Faces[FaceIndex].m_Position];
		VectPosition = Vector3f(Position[0], Position[2], Position[1] );
	}

	if ( m_Normals )
	{
		float*  Normal = m_Normals[m_Faces[FaceIndex].m_Normal];
		VectNormal = Vector3f (Normal[0], Normal[2], Normal[1] );
	}

	if ( m_TexCoords )
	{
		float*  TexCoord = m_TexCoords[m_Faces[FaceIndex].m_TexCoord];
		VectTexCoord = Vector2f (TexCoord[0], TexCoord[1]);
	}

	VertexBuffer[VertexIndex].m_Position	= VectPosition;
	VertexBuffer[VertexIndex].m_Normal		= VectNormal;
	VertexBuffer[VertexIndex].m_TexCoord	= VectTexCoord;
}

ResourceResult	 MeshLoader::FillFacesArray	(TiXmlNode* TrianglesNode)
{
	int iNbSemantics = 3;

	int iPositionOffset = -1,
		iNormalOffset   = -1,
		iTexCoordOffset = -1;

	int iPositionIndex = -1,
		iNormalIndex   = -1,
		iTexCoordIndex = -1;

	int iStartData = 0,
		iDataSize = 0;

	TiXmlNode* node = TrianglesNode;
	TiXmlString sSemantic;

	node = node->FirstChild("input");
	while (node)
	{
		sSemantic = node->ToElement()->Attribute("semantic");

		if		( strcmp(sSemantic.c_str() , "VERTEX") == 0 )
			node->ToElement()->Attribute("offset", &iPositionOffset);
		else if ( strcmp(sSemantic.c_str() , "NORMAL") == 0 )
			node->ToElement()->Attribute("offset", &iNormalOffset);
		else if ( strcmp(sSemantic.c_str() , "TEXCOORD") == 0 )
			node->ToElement()->Attribute("offset", &iTexCoordOffset);

		node = node->NextSiblingElement("input");
	}

	if ( m_iNbFaces )
	{
		m_Faces		   = new FaceVertex[m_iNbFaces*NbFaceVertices];

		node = TrianglesNode;
		node = node->FirstChild("p");


		const char* TrianglesText = node->ToElement()->GetText();

		for (int i = 0 ; i < m_iNbFaces*iNbSemantics ; i += 2*iNbSemantics)
		{
			for (int j = 0 ; j < iNbSemantics ; j++)
			{
				if		( j == iPositionOffset	&& iPositionOffset >= 0)
					iPositionIndex = atoi(&TrianglesText[i+j*2]);
				else if ( j == iNormalOffset	&& iNormalOffset >= 0)
					iNormalIndex = atoi(&TrianglesText[i+j*2]);
				else if ( j == iTexCoordOffset	&& iTexCoordOffset >= 0)
					iTexCoordIndex = atoi(&TrianglesText[i+j*2]);
			}
		}

		for (int i = 0 ; i < m_iNbFaces*NbFaceVertices ; i++)
		{
			for (int j = 0 ; j < iNbSemantics ; j++)
			{
				iDataSize = 0;
				while (TrianglesText[iStartData+iDataSize] != ' ')
					iDataSize ++;

				char* cData = new char[iDataSize];

				for (int k=0 ; k < iDataSize ; k++)
					cData[k] = TrianglesText[iStartData +k];

				if		( j == iPositionOffset	&& iPositionOffset >= 0)
					iPositionIndex = atoi(cData);
				else if ( j == iNormalOffset	&& iNormalOffset >= 0)
					iNormalIndex = atoi(cData);
				else if ( j == iTexCoordOffset	&& iTexCoordOffset >= 0)
					iTexCoordIndex = atoi(cData);

				delete [] cData;

				iStartData += iDataSize+1;
			}

			m_Faces [i].m_Position = iPositionIndex;
			m_Faces [i].m_Normal   = iNormalIndex;
			m_Faces [i].m_TexCoord = iTexCoordIndex;
		}
	}
	else	return RES_FAILED;

	return RES_SUCCEED;
}
