#include "MeshLoader.h"
#include "../Renderer/Renderer.h"

//===========================================================================//
// Constructeur MeshLoader                                                   //
//===========================================================================//
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

	m_Skinned		= false;
	m_iNbWeights	= 0;
	m_Weights		= NULL;

	for(int i=0; i<3; i++)
		m_OffsetPosition[i]=0;
	for(int i=0; i<4; i++)
		m_OffsetRotation[i]=0;
	for(int i=0; i<3; i++)
		m_OffsetScale[i]=1;

	m_Faces		= NULL;

	m_SkinnedVertices	= NULL;
}

//===========================================================================//
// Destructeur MeshLoader                                                    //
//===========================================================================//
MeshLoader::~MeshLoader()
{
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

	if (m_Weights)
		delete [] m_Weights;


	if (m_Faces)
		delete [] m_Faces;

	if (m_SkinnedVertices)
		delete [] m_SkinnedVertices;
}

//===========================================================================//
// Chargement de la ressource                                                //
//===========================================================================//
ResourceResult MeshLoader::Load(const char *sMeshPath,  Vertex *&VertexBuffer, int *&IndexBuffer, int &iNbVertices, int &iNbIndex, IDirect3DVertexDeclaration9* &vertdecl,
								Vector3f &Position, Vector4f &Rotation, Vector3f &Scale)
{
	TiXmlDocument meshFile( sMeshPath );
	if (!meshFile.LoadFile ())
	{
		std::string errorMsg = "Erreur de chargement du fichier ";
		errorMsg += sMeshPath;
		MessageBox(NULL, errorMsg.c_str(), "Chargement de Mesh", MB_ICONERROR);
		return RES_FAILED;
	}

	TiXmlNode* rootNode;

	rootNode =  meshFile.FirstChild( "COLLADA" );  // Get Root Node

	FillArrays (rootNode, VertexBuffer, IndexBuffer);

	Position =	Vector3f(m_OffsetPosition[0],	m_OffsetPosition[2],	m_OffsetPosition[1]); 
	Rotation =	Vector4f(abs(m_OffsetRotation[0]),	abs(m_OffsetRotation[2]),	abs(m_OffsetRotation[1]), m_OffsetRotation[3]);
	Scale	 =	Vector3f(m_OffsetScale[0],		m_OffsetScale[2],		m_OffsetScale[1]); 

	iNbVertices	= m_iNbVertices;
	iNbIndex	= m_iNbFaces*NbFaceVertices;

	Renderer::GetInstance()->m_pd3dDevice->CreateVertexDeclaration(&DxElements[0], &vertdecl);

	return RES_SUCCEED;
}

//===========================================================================//
// Rempli les tableaux de données                                            //
//===========================================================================//
ResourceResult MeshLoader::FillArrays	(TiXmlNode* rootNode,  Vertex *&VertexBuffer, int *&IndexBuffer)
{
	TiXmlNode* node;
	TiXmlNode* MeshNode;
	TiXmlNode* SkinNode;

	string sPositionsId ("positions");
	string sNormalsId	("normals");
	string sTexCoordsId ("channel1");

	string sWeightsId	("weights");
	std::vector<int> Offset(4, 0);

	int VertexDeclarationOffset = 0;

	//----------------------------------- Skinning -----------------------------------//
	if(rootNode) {
		node =  rootNode->FirstChild( "library_controllers" );
		if(node) {
			node =  node->FirstChild( "controller" );
			if(node) {
				node =  node->FirstChild( "skin" );
				SkinNode = node;
				if(node) {
					node =  node->FirstChild( "source" );		// Recherche la première balise "source"
					while (node)
					{
						string sId (node->ToElement()->Attribute("id"));		// vérification de l'id
						size_t SpliterPlace = sId.rfind("-")+1;

						if (SpliterPlace != sId.npos)
							if (sId.compare( SpliterPlace, sWeightsId.length(), sWeightsId) == 0)
								ExtractArrayDatas (node, m_Weights, m_iNbWeights, m_iNbWeights);

						node = node->NextSibling( "source" );
					}

					node = SkinNode;

					node =  node->FirstChild( "vertex_weights");
					if (node)
						FillSkinArray (node);

					m_Skinned = true;
				}
			}
		}
		//--------------------------------------------------------------------------------//

		//----------------------------------- Géometrie ----------------------------------//
		node =  rootNode->FirstChild( "library_geometries" );
		if(node) 
		{
			node =  node->FirstChild( "geometry" );
			if(node) 
			{
				node =  node->FirstChild( "mesh" );
				MeshNode = node;
				if(node) {
					node =  node->FirstChild( "source" );		// Recherche la première balise "source"

					D3DVERTEXELEMENT9 CurrentElement;

					while (node)
					{
						string sId (node->ToElement()->Attribute("id"));		// vérification de l'id
						size_t SpliterPlace = sId.rfind("-")+1;

						if (SpliterPlace != sId.npos)
						{
							CurrentElement.Stream = 0;
							CurrentElement.Method = D3DDECLMETHOD_DEFAULT;

							if (sId.compare( SpliterPlace, sPositionsId.length(), sPositionsId) == 0)
							{
								ExtractArrayDatas (node, m_Positions, m_iNbPositions, m_iNbVertices);
								CurrentElement.Offset = VertexDeclarationOffset;
								CurrentElement.Method = D3DDECLMETHOD_DEFAULT;
								CurrentElement.Usage = D3DDECLUSAGE_POSITION;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT3;

								VertexDeclarationOffset += 3*4;
							}
							else if (sId.compare( SpliterPlace, sNormalsId.length(), sNormalsId) == 0)
							{
								ExtractArrayDatas (node, m_Normals, m_iNbNormals, m_iNbVertices);
								CurrentElement.Offset = VertexDeclarationOffset;
								CurrentElement.Usage = D3DDECLUSAGE_NORMAL;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT3;

								VertexDeclarationOffset += 3*4;
							}
							else if (sId.compare( SpliterPlace, sTexCoordsId.length(), sTexCoordsId) == 0)
							{
								ExtractArrayDatas (node, m_TexCoords, m_iNbTexCoords, m_iNbVertices);
								CurrentElement.Offset = VertexDeclarationOffset;
								CurrentElement.Usage = D3DDECLUSAGE_TEXCOORD;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT2;

								VertexDeclarationOffset += 2*4;
							}

							DxElements.push_back(CurrentElement);
						}

						node = node->NextSibling( "source" );
					}

					if (m_Skinned)
					{
						CurrentElement.Offset = VertexDeclarationOffset;
						CurrentElement.Usage = D3DDECLUSAGE_BLENDINDICES;
						CurrentElement.UsageIndex = 0;
						CurrentElement.Type = D3DDECLTYPE_FLOAT4;

						VertexDeclarationOffset += 4*4;

						DxElements.push_back(CurrentElement);


						CurrentElement.Offset = VertexDeclarationOffset;
						CurrentElement.Usage = D3DDECLUSAGE_BLENDWEIGHT;
						CurrentElement.UsageIndex = 0;
						CurrentElement.Type = D3DDECLTYPE_FLOAT4;

						VertexDeclarationOffset += 4*4;

						DxElements.push_back(CurrentElement);
					}

					node = MeshNode;

					node =  node->FirstChild( "triangles");
					if (node)
						FillVBArray (node, VertexBuffer, IndexBuffer);

				}
			}
		}
		//--------------------------------------------------------------------------------//

		//-------------------------------- Transformation --------------------------------//
		node =  rootNode->FirstChild()->NextSibling("library_visual_scenes");
		if(node) 
		{
			node = node->FirstChild( "visual_scene" );
			if(node)
			{
				node = node->FirstChild( "node" );
				if(node)
				{
					const char* ArrayText;
					node = node->FirstChild( "translate" );
					if(node)
					{
						ArrayText=node->ToElement()->GetText();
						ConvertTextToArray(ArrayText, m_OffsetPosition, 3);

						node = node->NextSibling( "rotate" );
						if(node)
						{
							ArrayText=node->ToElement()->GetText();
							ConvertTextToArray(ArrayText, m_OffsetRotation, 4);

							node = node->NextSibling( "scale" );
							if(node)
							{
								ArrayText=node->ToElement()->GetText();
								ConvertTextToArray(ArrayText, m_OffsetScale, 3);
							}
						}
					}
				}
			}
		}
	}
	//--------------------------------------------------------------------------------//

	D3DVERTEXELEMENT9 EndElement = D3DDECL_END();
	DxElements.push_back(EndElement);

	return RES_SUCCEED;
}

//===========================================================================//
// Extrait les données liées à un tableau                                    //
//===========================================================================//
ResourceResult MeshLoader::ExtractArrayDatas	(TiXmlNode* sourceNode, float** &Array, int &iNbElements, int &iNbMaxElements)
{
	TiXmlNode* node = sourceNode;			// Enregistrement de la balise source

	const char* ArrayText;

	int iXMLNbElements = 0;
	int iStride = 0;


	node =  node->FirstChild( "technique_common" );
	if(node) {
		node =  node->FirstChild( "accessor" );
		if(node) {

			node->ToElement()->Attribute("count", &iXMLNbElements);		// Stockage du nombre de vertices
			if (iNbMaxElements < iXMLNbElements)	iNbMaxElements = iXMLNbElements;	// Enregistrement du plus grand nombre de vertices

			iNbElements = iXMLNbElements;

			node->ToElement()->Attribute("stride", &iStride);			// Stockage de la taille du tableau				

			if (!Array && iXMLNbElements && iStride)			// Si le tableau n'a pas été alloué et que le nombre de vertices et la taille sont connus
			{
				Array = new float*[iXMLNbElements];				// Le tableau est alloué
				for (int i=0 ; i<iXMLNbElements ; i++)
					Array[i] = new float[iStride];
			}
		}
	}					

	node = sourceNode;
	node =  node->FirstChild( "float_array" );
	if(node) {
		ArrayText = node->ToElement()->GetText();

		ConvertTextToArray(ArrayText, Array, iXMLNbElements, iStride);

		//delete text;
	}

	return RES_FAILED;
}

//===========================================================================//
// Copie les données d'une chaîne caractères provenant du fichier XML dans   //
// un tableau.																 //
//===========================================================================//
ResourceResult MeshLoader::ConvertTextToArray(const char* ArrayText, float** Array, int iCount, int iStride)
{
	float* tmpArray = new float[iCount*iStride];
	ConvertTextToArray(ArrayText, tmpArray, iCount*iStride);

	for (int i = 0 ; i < iCount ; i++)
		for (int j = 0 ; j < iStride ; j++)
			Array[i][j] = tmpArray[i*iStride + j];

	delete [] tmpArray;

	return RES_SUCCEED;
}

ResourceResult	 MeshLoader::ConvertTextToArray (const char* ArrayText, float*  Array, int iCount)
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

ResourceResult	 MeshLoader::ConvertTextToArray (const char* ArrayText, int*  Array, int iCount)
{
	int id=0;
	int dec=0;

	int result	= 0;
	int signe		= 1;

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
		}
		Array[i]=result*signe;
		id=id+dec+1;
		dec=0, result=0, signe=1;
	}

	return RES_SUCCEED;
}


//===========================================================================//
// Remplit vertex buffer et index buffer                                     //
//===========================================================================//
ResourceResult MeshLoader::FillVBArray	(TiXmlNode* TrianglesNode,  Vertex *&VertexBuffer, int *&IndexBuffer)
{
	TiXmlNode* node = TrianglesNode;			// Enregistrement de la balise triangles

	node->ToElement()->Attribute("count", &m_iNbFaces);

	FillFacesArray ( node );


	IndexBuffer  = new int [m_iNbFaces*NbFaceVertices];

	int FaceIndex;
	int nbvert=0;

	for (int i = 0 ; i < m_iNbFaces*NbFaceVertices ; i ++)
	{
		FaceIndex = 0;
		while (FaceIndex < i && m_Faces[FaceIndex] != m_Faces[i] )
			FaceIndex ++;

		if ( FaceIndex == i )
		{
			nbvert++;
		}
	}
	m_iNbVertices=nbvert;

	if (!m_Skinned)
		VertexBuffer = new Vertex [m_iNbVertices];
	else
		VertexBuffer = new SkinnedVertex [m_iNbVertices];

	int vertexCount=0;

	for (int i = 0 ; i < m_iNbFaces*NbFaceVertices ; i ++)
	{
		FaceIndex = 0;
		while (FaceIndex < i && m_Faces[FaceIndex] != m_Faces[i] )
			FaceIndex ++;

		if ( FaceIndex < i )
		{
			IndexBuffer[i] = IndexBuffer[FaceIndex];
		}
		else
		{
			IndexBuffer[i] = vertexCount;
			FillVertex(vertexCount, i, VertexBuffer, IndexBuffer);
			vertexCount++;
		}
	}

	return RES_SUCCEED;
}

/****************************************************************************************************************************/
ResourceResult	MeshLoader::FillSkinArray (TiXmlNode* VertexWeightsNode)
{
	TiXmlNode* node = VertexWeightsNode;			// Enregistrement de la balise vertex_weights

	node->ToElement()->Attribute("count", &m_iNbSkinnedVertices);

	int iNbSemantics	= 2;

	int iJointOffset	= -1,
		iWeightOffset   = -1;

	int iJointIndex		= -1,
		iWeightIndex	= -1;

	TiXmlString sSemantic;

	node = node->FirstChild("input");
	while (node)
	{
		sSemantic = node->ToElement()->Attribute("semantic");

		if		( strcmp(sSemantic.c_str() , "JOINT") == 0 )
			node->ToElement()->Attribute("offset", &iJointOffset);
		else if ( strcmp(sSemantic.c_str() , "WEIGHT") == 0 )
			node->ToElement()->Attribute("offset", &iWeightOffset);

		node = node->NextSiblingElement("input");
	}

	if ( m_iNbSkinnedVertices )
	{
		m_SkinnedVertices	= new VertexSkinning[m_iNbSkinnedVertices];

		node = VertexWeightsNode;
		node = node->FirstChild("vcount");


		const char* NbWeightsText		= node->ToElement()->GetText();
		int* NbWeightsArray = new int [m_iNbSkinnedVertices];
		ConvertTextToArray(NbWeightsText, NbWeightsArray, m_iNbSkinnedVertices);

		int NbWeightsUsed = 0;
		for (int i=0 ; i<m_iNbSkinnedVertices ; i++)
			NbWeightsUsed += NbWeightsArray[i];

		node = node->NextSiblingElement("v");
		const char* JointsWeightsText	= node->ToElement()->GetText();
		int* JointsWeightsArray = new int [NbWeightsUsed];
		ConvertTextToArray(JointsWeightsText, JointsWeightsArray, NbWeightsUsed);

		int iCurrentSkinnedVertex = 0;
		int iJointsWeightsPos = 0;

		for (int i = 0 ; i <  m_iNbSkinnedVertices ; i++)
		{
			iJointsWeightsPos = 0;
			memset(m_SkinnedVertices[iCurrentSkinnedVertex].m_Joint, 0, sizeof(m_SkinnedVertices[iCurrentSkinnedVertex].m_Joint));
			memset(m_SkinnedVertices[iCurrentSkinnedVertex].m_Weight, 0, sizeof(m_SkinnedVertices[iCurrentSkinnedVertex].m_Weight));
			for (int j = 0 ; j < NbWeightsArray[i]; j++)
			{
				if (j < NbWeightsMax)
				{
					m_SkinnedVertices[iCurrentSkinnedVertex].m_Joint[j]		= JointsWeightsArray[iJointsWeightsPos];
					m_SkinnedVertices[iCurrentSkinnedVertex].m_Weight[j]	= m_Weights[JointsWeightsArray[iJointsWeightsPos+1]][0];
				}

				iJointsWeightsPos +=2;
			}
			iCurrentSkinnedVertex ++;
		}

		delete [] NbWeightsArray;
		delete [] JointsWeightsArray;
	}
	else	return RES_FAILED;

	return RES_SUCCEED;
}

//===========================================================================//
// Remplit vertex buffer				                                     //
//===========================================================================//
void MeshLoader::FillVertex(int VertexIndex, int FaceIndex,  Vertex *&VertexBuffer, int *&IndexBuffer)
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

	if (m_Skinned)
	{
		((SkinnedVertex*)VertexBuffer)[VertexIndex].m_Bones.x	= (float) m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Joint[0];
		((SkinnedVertex*)VertexBuffer)[VertexIndex].m_Bones.y	= (float) m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Joint[1];
		((SkinnedVertex*)VertexBuffer)[VertexIndex].m_Bones.z	= (float) m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Joint[2];
		((SkinnedVertex*)VertexBuffer)[VertexIndex].m_Bones.w	= (float) m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Joint[3];

		((SkinnedVertex*)VertexBuffer)[VertexIndex].m_Weights.x	= (float) m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Weight[0];
		((SkinnedVertex*)VertexBuffer)[VertexIndex].m_Weights.y	= (float) m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Weight[1];
		((SkinnedVertex*)VertexBuffer)[VertexIndex].m_Weights.z	= (float) m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Weight[2];
		((SkinnedVertex*)VertexBuffer)[VertexIndex].m_Weights.w	= (float) m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Weight[3];
	}
}

//===========================================================================//
// Remplit index buffer					                                     //
//===========================================================================//
ResourceResult MeshLoader::FillFacesArray(TiXmlNode* TrianglesNode)
{
	int iNbSemantics = 3;

	int iPositionOffset = -1,
		iNormalOffset   = -1,
		iTexCoordOffset = -1;

	int iPositionIndex = -1,
		iNormalIndex   = -1,
		iTexCoordIndex = -1;

	int iStartData	= 0,
		iDataSize	= 0;

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
				while (TrianglesText[iStartData+iDataSize] != ' ' && TrianglesText[iStartData+iDataSize] != '\0')
					iDataSize ++;

				char cData[15]= {0};

				for (int k=0 ; k < iDataSize ; k++)
					cData[k] = TrianglesText[iStartData +k];

				if		( j == iPositionOffset	&& iPositionOffset >= 0)
					iPositionIndex = atoi(cData);
				else if ( j == iNormalOffset	&& iNormalOffset >= 0)
					iNormalIndex = atoi(cData);
				else if ( j == iTexCoordOffset	&& iTexCoordOffset >= 0)
					iTexCoordIndex = atoi(cData);

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
