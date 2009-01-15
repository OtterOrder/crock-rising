#ifndef		_Mesh_Loader_H
#define		_Mesh_Loader_H

//******************************************************************
#include	<string>
#include	<vector>

#include	<TinyXml/tinyxml.h>
#include	"Core/Types/Vector.h"

#include	"Resource.h"

using namespace std;
//******************************************************************

#define NbFaceVertices 3

struct Vertex
{
	Vector3f m_Position;
	Vector3f m_Normal;
	Vector2f m_TexCoord;
};

struct FaceVertex
{
	int m_Position;
	int m_Normal;
	int m_TexCoord;

	bool operator == (const FaceVertex &faceVertex)	{ return ( m_Position == faceVertex.m_Position && m_Normal == faceVertex.m_Normal && m_TexCoord == faceVertex.m_TexCoord ); }
	bool operator != (const FaceVertex &faceVertex)	{ return ! (*this == faceVertex); }
};

class MeshLoader
{
	int			m_iNbVertices,
				m_iNbFaces;

	float		**m_Positions,
				**m_Normals,
				**m_TexCoords;

	int			m_iNbPositions,
				m_iNbNormals,
				m_iNbTexCoords;

	float		m_OffsetPosition[3];
	float		m_OffsetRotation[4];
	float		m_OffsetScale[3];

	FaceVertex	*m_Faces;
	std::vector<D3DVERTEXELEMENT9>	 DxElements; // Vecteur d'éléments du vertex

public:

			MeshLoader(void);
	virtual	~MeshLoader(void);

	ResourceResult	Load				(const char *sMeshPath,  Vertex *&VertexBuffer, int *&IndexBuffer, int &iNbVertices, int &iNbIndex, IDirect3DVertexDeclaration9* &vertdecl, 
										D3DXVECTOR3 &Position, D3DXVECTOR4 &Rotation, D3DXVECTOR3 &Scale);

	ResourceResult	FillArrays			(TiXmlNode* rootNode,  Vertex *&VertexBuffer, int *&IndexBuffer);		// Remplit les tableaux de données
	ResourceResult	ExtractArrayDatas	(TiXmlNode* sourceNode, float** &Array, int &iNbElements);								// Extrait les données d'une balise
	ResourceResult	ConvertTextToArray	(const char* ArrayText, float** &Array, int iCount, int iStride);		// Rempli un tableau à l'aide d'un texte
	ResourceResult	ConvertTextToArray	(const char* ArrayText, float* Array, int iCount);

	ResourceResult	FillVBArray			(TiXmlNode* TrianglesNode,  Vertex *&VertexBuffer, int *&IndexBuffer);	// Remplit le tableau de vertex
	void			FillVertex			(int VertexIndex, int FaceIndex,  Vertex *&VertexBuffer, int *&IndexBuffer);
	ResourceResult	FillFacesArray		(TiXmlNode* TrianglesNode);												// Remplit le tableau de faces
};

#endif	// _Mesh_Loader_H
