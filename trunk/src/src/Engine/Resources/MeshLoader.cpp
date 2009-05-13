#include "MeshLoader.h"
#include "../Renderer/Renderer.h"


//******************************************************************

VertexBuffer::VertexBuffer()
{
	m_VertexType = None;
	m_NbVertices = 0;
	m_Size		 = 0;
	m_Datas		 = NULL;
}

VertexBuffer::~VertexBuffer()
{
	if (m_Datas)
		delete [] m_Datas;
}

//******************************************************************

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

	m_iNbWeights		= 0;
	m_Weights			= NULL;
	m_SkinnedVertices	= NULL;

	m_iNbTangents		= 0;
	m_Tangents			= NULL;
	m_iNbBinormals		= 0;
	m_Binormals			= NULL;

	m_Faces		= NULL;
}

//===========================================================================//
// Destructeur MeshLoader                                                    //
//===========================================================================//
MeshLoader::~MeshLoader()
{
	// Vertex
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

	// Skinning
	if (m_Weights)
	{
		for (int i = 0 ; i < m_iNbWeights ; i++)
			delete m_Weights[i];
		delete [] m_Weights;
	}

	if (m_SkinnedVertices)
		delete [] m_SkinnedVertices;

	// Normal Mapping
	if (m_Tangents)
	{
		for (int i = 0 ; i < m_iNbTangents ; i++)
			delete m_Tangents[i];
		delete [] m_Tangents;
	}
	if (m_Binormals)
	{
		for (int i = 0 ; i < m_iNbBinormals ; i++)
			delete m_Binormals[i];
		delete [] m_Binormals;
	}

	// Faces
	if (m_Faces)
		delete [] m_Faces;
}

//===========================================================================//
// Chargement de la ressource                                                //
//===========================================================================//
ResourceResult MeshLoader::Load(const char *sMeshPath,  VertexBuffer& _VertexBuffer, int *&IndexBuffer, int &iNbIndex, IDirect3DVertexDeclaration9* &vertdecl)
{
	TiXmlDocument meshFile( sMeshPath );
	if (!meshFile.LoadFile ())
	{
		std::string errorMsg = "Erreur de chargement du fichier ";
		errorMsg += sMeshPath;
		MessageBox(NULL, errorMsg.c_str(), "Chargement de Mesh", MB_ICONERROR);
		return RES_FAILED;
	}

	_VertexBuffer.m_Datas		= NULL;
	_VertexBuffer.m_VertexType	= VertexBuffer::None;

	TiXmlNode* rootNode;

	rootNode =  meshFile.FirstChild( "COLLADA" );  // Get Root Node

	FillArrays (rootNode, _VertexBuffer, IndexBuffer);

	_VertexBuffer.m_NbVertices	= m_iNbVertices;

	u32 vertexSize = 0;
	switch (_VertexBuffer.m_VertexType)
	{
		case VertexBuffer::Default:
			vertexSize = sizeof(Vertex);
			break;

		case VertexBuffer::Skinned:
			vertexSize = sizeof(SkinnedVertex);
			break;

		case VertexBuffer::NormalMapped:
			vertexSize = sizeof(NormalMappedVertex);
			break;
	}

	_VertexBuffer.m_Size = _VertexBuffer.m_NbVertices*vertexSize;

	iNbIndex	= m_iNbFaces*NbFaceVertices;

	Renderer::GetInstance()->m_pd3dDevice->CreateVertexDeclaration(&DxElements[0], &vertdecl);

	return RES_SUCCEED;
}

//===========================================================================//
// Rempli les tableaux de données                                            //
//===========================================================================//
ResourceResult MeshLoader::FillArrays	(TiXmlNode* rootNode,  VertexBuffer& _VertexBuffer, int *&IndexBuffer)
{
	TiXmlNode* node;
	TiXmlNode* MeshNode;
	TiXmlNode* SkinNode;

	string sPositionsId ("positions");
	string sNormalsId	("normals");
	string sTexCoordsId ("channel1");

	string sWeightsId	("weights");

	string sTangentsId	("tangents");
	string sBinormalsId	("binormals");

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

					_VertexBuffer.m_VertexType = VertexBuffer::Skinned;
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

					if (_VertexBuffer.m_VertexType	== VertexBuffer::None)
						_VertexBuffer.m_VertexType	= VertexBuffer::Default;

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
								CurrentElement.Type = D3DDECLTYPE_FLOAT4;

								VertexDeclarationOffset += 4*4;
								DxElements.push_back(CurrentElement);
							}
							else if (sId.compare( SpliterPlace, sNormalsId.length(), sNormalsId) == 0)
							{
								ExtractArrayDatas (node, m_Normals, m_iNbNormals, m_iNbVertices);
								CurrentElement.Offset = VertexDeclarationOffset;
								CurrentElement.Usage = D3DDECLUSAGE_NORMAL;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT3;

								VertexDeclarationOffset += 3*4;
								DxElements.push_back(CurrentElement);
							}
							else if (sId.compare( SpliterPlace, sTexCoordsId.length(), sTexCoordsId) == 0)
							{
								ExtractArrayDatas (node, m_TexCoords, m_iNbTexCoords, m_iNbVertices);
								CurrentElement.Offset = VertexDeclarationOffset;
								CurrentElement.Usage = D3DDECLUSAGE_TEXCOORD;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT2;

								VertexDeclarationOffset += 2*4;
								DxElements.push_back(CurrentElement);
							}
							// Normal Mapping
							else if (sId.compare( SpliterPlace, sTangentsId.length(), sTangentsId) == 0)
							{
								
								ExtractArrayDatas (node, m_Tangents, m_iNbTangents, m_iNbVertices);
								CurrentElement.Offset = VertexDeclarationOffset;
								CurrentElement.Usage = D3DDECLUSAGE_TANGENT;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT3;

								VertexDeclarationOffset += 3*4;
								DxElements.push_back(CurrentElement);

								//if (_VertexBuffer.m_VertexType	== VertexBuffer::Default)
									_VertexBuffer.m_VertexType	= VertexBuffer::NormalMapped;
							}
							else if (sId.compare( SpliterPlace, sBinormalsId.length(), sBinormalsId) == 0)
							{
								
								ExtractArrayDatas (node, m_Binormals, m_iNbBinormals, m_iNbVertices);
								CurrentElement.Offset = VertexDeclarationOffset;
								CurrentElement.Usage = D3DDECLUSAGE_BINORMAL;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT3;

								VertexDeclarationOffset += 3*4;
								DxElements.push_back(CurrentElement);

								_VertexBuffer.m_VertexType	= VertexBuffer::NormalMapped;
							}
						}

						node = node->NextSibling( "source" );
					}

					if (_VertexBuffer.m_VertexType == VertexBuffer::Skinned)
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
						FillVBArray (node, _VertexBuffer, IndexBuffer);

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
ResourceResult MeshLoader::FillVBArray	(TiXmlNode* TrianglesNode,  VertexBuffer& _VertexBuffer, int *&IndexBuffer)
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

	switch (_VertexBuffer.m_VertexType)
	{
		case VertexBuffer::Default:
			_VertexBuffer.m_Datas = new Vertex [m_iNbVertices];
			break;

		case VertexBuffer::Skinned:
			_VertexBuffer.m_Datas = new SkinnedVertex [m_iNbVertices];
			break;

		case VertexBuffer::NormalMapped:
			_VertexBuffer.m_Datas = new NormalMappedVertex [m_iNbVertices];
			break;
	}

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
			FillVertex(vertexCount, i, _VertexBuffer, IndexBuffer);
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
		int* JointsWeightsArray = new int [NbWeightsUsed*2];
		ConvertTextToArray(JointsWeightsText, JointsWeightsArray, NbWeightsUsed*2);

		int iCurrentSkinnedVertex = 0;
		int iJointsWeightsPos = 0;

		//iJointsWeightsPos = 0;

		for (int i = 0 ; i <  m_iNbSkinnedVertices ; i++)
		{
			memset(m_SkinnedVertices[iCurrentSkinnedVertex].m_Joint, 0, sizeof(m_SkinnedVertices[iCurrentSkinnedVertex].m_Joint));
			memset(m_SkinnedVertices[iCurrentSkinnedVertex].m_Weight, 0, sizeof(m_SkinnedVertices[iCurrentSkinnedVertex].m_Weight));
			for (int j = 0 ; j < NbWeightsArray[i]; j++)
			{
				if (j < NbWeightsMax)
				{
					m_SkinnedVertices[iCurrentSkinnedVertex].m_Joint[j]		= (float)JointsWeightsArray[iJointsWeightsPos];
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
void MeshLoader::FillVertex(int VertexIndex, int FaceIndex,  VertexBuffer& _VertexBuffer, int *&IndexBuffer)
{
	Vector3f VectPosition	(0.f, 0.f, 0.f);
	Vector3f VectNormal		(0.f, 0.f, 0.f);
	Vector2f VectTexCoord	(0.f, 0.f);


	if ( m_Positions )
	{
		float*  Position = m_Positions[m_Faces[FaceIndex].m_Position];
		VectPosition = Vector3f(Position[0], Position[1], Position[2]);
	}

	if ( m_Normals )
	{
		float*  Normal = m_Normals[m_Faces[FaceIndex].m_Normal];
		VectNormal = Vector3f (Normal[0], Normal[1], Normal[2]);
	}

	if ( m_TexCoords )
	{
		float*  TexCoord = m_TexCoords[m_Faces[FaceIndex].m_TexCoord];
		VectTexCoord = Vector2f (TexCoord[0], TexCoord[1]);
	}

	u32 vertexSize = 0;
	switch (_VertexBuffer.m_VertexType)
	{
	case VertexBuffer::Default:
		vertexSize = sizeof(Vertex);
		break;

	case VertexBuffer::Skinned:
		vertexSize = sizeof(SkinnedVertex);
		break;

	case VertexBuffer::NormalMapped:
		vertexSize = sizeof(NormalMappedVertex);
		break;
	}

	Vertex* pDVertexBuffer = (Vertex*)((u8*)_VertexBuffer.m_Datas + VertexIndex*vertexSize);

	pDVertexBuffer->m_Position	= Vector4f( VectPosition, 1.f);
	pDVertexBuffer->m_Normal	= VectNormal;
	pDVertexBuffer->m_TexCoord	= VectTexCoord;

	if (_VertexBuffer.m_VertexType == VertexBuffer::Skinned)
	{
		SkinnedVertex* pSVertexBuffer = (SkinnedVertex*)pDVertexBuffer;

		pSVertexBuffer->m_Bones		= m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Joint;
		pSVertexBuffer->m_Weights	= m_SkinnedVertices[m_Faces[FaceIndex].m_Position].m_Weight;
	}
	else
	if (_VertexBuffer.m_VertexType == VertexBuffer::NormalMapped)
	{
		Vector3f float3	(0.f, 0.f, 0.f);

		NormalMappedVertex* pSVertexBuffer = (NormalMappedVertex*)pDVertexBuffer;

		float*  Tangeant = m_Tangents[m_Faces[FaceIndex].m_Tangent];
		pSVertexBuffer->m_Tangent	= Vector3f (Tangeant[0], Tangeant[1], Tangeant[2]);

		float*  Binormal = m_Binormals[m_Faces[FaceIndex].m_Binormal];
		pSVertexBuffer->m_Binormal	= Vector3f (Binormal[0], Binormal[1], Binormal[2]);
	}
}

//===========================================================================//
// Remplit index buffer					                                     //
//===========================================================================//
ResourceResult MeshLoader::FillFacesArray(TiXmlNode* TrianglesNode)
{
	int iNbSemantics = 0;

	int iPositionOffset = -1,
		iNormalOffset   = -1,
		iTexCoordOffset = -1,
		iTangentOffset	= -1,
		iBinormalOffset	= -1;

	int iPositionIndex	= -1,
		iNormalIndex	= -1,
		iTexCoordIndex	= -1,
		iTangentIndex	= -1,
		iBinormalIndex	= -1;

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
		else if ( strcmp(sSemantic.c_str() , "TEXTANGENT") == 0 )
			node->ToElement()->Attribute("offset", &iTangentOffset);
		else if ( strcmp(sSemantic.c_str() , "TEXBINORMAL") == 0 )
			node->ToElement()->Attribute("offset", &iBinormalOffset);

		node = node->NextSiblingElement("input");
		iNbSemantics ++;
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
				else if ( j == iTangentOffset	&& iTangentOffset >= 0)
					iTangentIndex = atoi(&TrianglesText[i+j*2]);
				else if ( j == iBinormalOffset	&& iBinormalOffset >= 0)
					iBinormalIndex = atoi(&TrianglesText[i+j*2]);
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
				else if ( j == iTangentOffset	&& iTangentOffset >= 0)
					iTangentIndex =  atoi(cData);
				else if ( j == iBinormalOffset	&& iBinormalOffset >= 0)
					iBinormalIndex =  atoi(cData);

				iStartData += iDataSize+1;
			}

			m_Faces[i].m_Position	= iPositionIndex;
			m_Faces[i].m_Normal		= iNormalIndex;
			m_Faces[i].m_TexCoord	= iTexCoordIndex;

			m_Faces[i].m_Tangent	= iTangentIndex;
			m_Faces[i].m_Binormal	= iBinormalIndex;
		}
	}
	else	return RES_FAILED;

	return RES_SUCCEED;
}
