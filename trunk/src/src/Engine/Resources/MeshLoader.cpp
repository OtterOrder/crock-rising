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

	for(int i=0; i<3; i++)
		m_OffsetPosition[i]=0;
	for(int i=0; i<4; i++)
		m_OffsetRotation[i]=0;
	for(int i=0; i<3; i++)
		m_OffsetScale[i]=1;

	m_Faces		= NULL;
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

	if (m_Faces)
		delete [] m_Faces;
}

//===========================================================================//
// Chargement de la ressource                                                //
//===========================================================================//
ResourceResult MeshLoader::Load(const char *sMeshPath,  Vertex *&VertexBuffer, int *&IndexBuffer, int &iNbVertices, int &iNbIndex, IDirect3DVertexDeclaration9* &vertdecl,
						 D3DXVECTOR3 &Position, D3DXVECTOR4 &Rotation, D3DXVECTOR3 &Scale)
{
	TiXmlDocument meshFile( sMeshPath );
	if (!meshFile.LoadFile ())
		return RES_FAILED;

	TiXmlNode* rootNode;

	rootNode =  meshFile.FirstChild( "COLLADA" );  // Get Root Node
    
	FillArrays (rootNode, VertexBuffer, IndexBuffer);

	Position=D3DXVECTOR3(m_OffsetPosition[0], m_OffsetPosition[2], m_OffsetPosition[1]); 
	Rotation=D3DXVECTOR4(m_OffsetRotation[0], m_OffsetRotation[2], m_OffsetRotation[1], m_OffsetRotation[3]);
	Scale=D3DXVECTOR3(m_OffsetScale[0], m_OffsetScale[2], m_OffsetScale[1]); 
	
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
	
	//const char * SourceId;

	/*string sPositionsId ("Position");
	string sNormalsId	("Normal0");
	string sTexCoordsId ("UV0");*/

	string sPositionsId ("positions");
	string sNormalsId	("normals");
	string sTexCoordsId ("channel1");

	std::vector<int> Offset(4, 0);


	if(rootNode) 
	{
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
					
					while (node)
					{
						string sId (node->ToElement()->Attribute("id"));		// vérification de l'id
						size_t SpliterPlace = sId.rfind("-")+1;

						if (SpliterPlace != sId.npos)
						{
							D3DVERTEXELEMENT9 CurrentElement;

							CurrentElement.Stream = 0;
							CurrentElement.Method = D3DDECLMETHOD_DEFAULT;
							
							if (sId.compare( SpliterPlace, sPositionsId.length(), sPositionsId) == 0)
							{
								ExtractArrayDatas (node, m_Positions, m_iNbPositions);
								CurrentElement.Offset = 0;
								CurrentElement.Method = D3DDECLMETHOD_DEFAULT;
								CurrentElement.Usage = D3DDECLUSAGE_POSITION;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT3;
							}
							else if (sId.compare( SpliterPlace, sNormalsId.length(), sNormalsId) == 0)
							{
								ExtractArrayDatas (node, m_Normals, m_iNbNormals);
								CurrentElement.Offset = 12;
								CurrentElement.Usage = D3DDECLUSAGE_NORMAL;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT3;
							}
							else if (sId.compare( SpliterPlace, sTexCoordsId.length(), sTexCoordsId) == 0)
							{
								ExtractArrayDatas (node, m_TexCoords, m_iNbTexCoords);
								CurrentElement.Offset = 24;
								CurrentElement.Usage = D3DDECLUSAGE_TEXCOORD;
								CurrentElement.UsageIndex = 0;
								CurrentElement.Type = D3DDECLTYPE_FLOAT2;
							}

							DxElements.push_back(CurrentElement);
							
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
	D3DVERTEXELEMENT9 EndElement = D3DDECL_END();
	DxElements.push_back(EndElement);

	return RES_SUCCEED;
}

//===========================================================================//
// Extrait les données liées à un tableau                                    //
//===========================================================================//
ResourceResult MeshLoader::ExtractArrayDatas	(TiXmlNode* sourceNode, float** &Array, int &iNbElements)
{
	TiXmlNode* node = sourceNode; // Enregistrement de la balise source

	const char* ArrayText;

	int iNbVertices = 0;
	int iStride = 0;

	
	node =  node->FirstChild( "technique_common" );
	if(node) {
		node =  node->FirstChild( "accessor" );
		if(node) {
			
			node->ToElement()->Attribute("count", &iNbVertices); // Stockage du nombre de vertices
			if (m_iNbVertices < iNbVertices)	m_iNbVertices = iNbVertices; // Enregistrement du plus grand nombre de vertices

			iNbElements = iNbVertices;

			node->ToElement()->Attribute("stride", &iStride); // Stockage de la taille du tableau				

			if (!Array && iNbVertices && iStride) // Si le tableau n'a pas été alloué et que le nombre de vertices et la taille sont connus
			{
				Array = new float*[iNbVertices]; // Le tableau est alloué
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

//===========================================================================//
// Copie les données d'une chaîne caractères provenant du fichier XML dans   //
// un tableau.																 //
//===========================================================================//
ResourceResult MeshLoader::ConvertTextToArray(const char* ArrayText, float** &Array, int iCount, int iStride)
{
	int id=0;
	int dec=0;

	float result	= 0;
	float signe		= 1;
	float mantisse	= 0.1f;

	for (int i = 0 ; i < iCount ; i++)
	{
		for (int j = 0 ; j < iStride ; j++)
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
			Array[i][j]=result*signe;
			if(Array[i][j]==0 && j!=2)
				int u=0;
			id=id+dec+1;
			dec=0, result=0, mantisse=0.1f, signe=1;
		}	
	}

	return RES_FAILED;
}

ResourceResult MeshLoader::ConvertTextToArray(const char* ArrayText, float* Array, int iCount)
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
	return RES_FAILED;
}


//===========================================================================//
// Remplit vertex buffer et index buffer                                     //
//===========================================================================//
ResourceResult MeshLoader::FillVBArray	(TiXmlNode* TrianglesNode,  Vertex *&VertexBuffer, int *&IndexBuffer)
{
	TiXmlNode* node = TrianglesNode; // Enregistrement de la balise triangles

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

	VertexBuffer = new Vertex [m_iNbVertices];

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

//===========================================================================//
// Remplit vertex buffer				                                     //
//===========================================================================//
void MeshLoader::FillVertex(int VertexIndex, int FaceIndex,  Vertex *&VertexBuffer, int *&IndexBuffer)
{
	D3DXVECTOR3 VectPosition	(0.f, 0.f, 0.f);
	D3DXVECTOR3 VectNormal		(0.f, 0.f, 0.f);
	D3DXVECTOR2 VectTexCoord	(0.f, 0.f);


	if ( m_Positions )
	{
		float*  Position = m_Positions[m_Faces[FaceIndex].m_Position];
		VectPosition = D3DXVECTOR3 (Position[0], Position[2], Position[1] );
	}

	if ( m_Normals )
	{
		float*  Normal = m_Normals[m_Faces[FaceIndex].m_Normal];
		VectNormal = D3DXVECTOR3 (Normal[0], Normal[2], Normal[1] );
	}

	if ( m_TexCoords )
	{
		float*  TexCoord = m_TexCoords[m_Faces[FaceIndex].m_TexCoord];
		VectTexCoord = D3DXVECTOR2 (TexCoord[0], TexCoord[1]);
	}

	VertexBuffer[VertexIndex].m_Position	= VectPosition;
	VertexBuffer[VertexIndex].m_Normal		= VectNormal;
	VertexBuffer[VertexIndex].m_TexCoord	= VectTexCoord;
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
