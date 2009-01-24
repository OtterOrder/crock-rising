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

#define NbFaceVertices	3
#define NbWeightsMax	4

struct Vertex
{
	Vector3f m_Position;
	Vector3f m_Normal;
	Vector2f m_TexCoord;
};

struct SkinnedVertex : public Vertex
{
	Vector4f m_Bones;
	Vector4f m_Weights;
};

struct FaceVertex
{
	int m_Position;
	int m_Normal;
	int m_TexCoord;

	bool operator == (const FaceVertex &faceVertex)	{ return ( m_Position == faceVertex.m_Position && m_Normal == faceVertex.m_Normal && m_TexCoord == faceVertex.m_TexCoord ); }
	bool operator != (const FaceVertex &faceVertex)	{ return ! (*this == faceVertex); }
};

struct VertexSkinning
{
	int		m_Joint[NbWeightsMax];
	float	m_Weight[NbWeightsMax];
};

//------------------------------------------------------------------

class MeshLoader
{
	int			m_iNbVertices,
		m_iNbFaces,
		m_iNbSkinnedVertices;

	float**		m_Positions,
		 **		m_Normals,
		 **		m_TexCoords;

	int			m_iNbPositions,
				m_iNbNormals,
				m_iNbTexCoords;

	float		m_OffsetPosition[3];
	float		m_OffsetRotation[4];
	float		m_OffsetScale[3];

	bool		m_Skinned;
	int			m_iNbWeights;
	float**		m_Weights;

	FaceVertex	*m_Faces;
	std::vector<D3DVERTEXELEMENT9>	 DxElements; // Vecteur d'éléments du vertex

	VertexSkinning*	m_SkinnedVertices;


public:

	MeshLoader(void);
	virtual	~MeshLoader(void);

	ResourceResult	Load				(const char *sMeshPath,  Vertex *&VertexBuffer, int *&IndexBuffer, int &iNbVertices, int &iNbIndex, IDirect3DVertexDeclaration9* &vertdecl, 
		D3DXVECTOR3 &Position, D3DXVECTOR4 &Rotation, D3DXVECTOR3 &Scale);

	ResourceResult	FillArrays					(TiXmlNode* rootNode,  Vertex *&VertexBuffer, int *&IndexBuffer);				// Remplit les tableaux de données
	ResourceResult	ExtractArrayDatas			(TiXmlNode* sourceNode, float** &Array, int &iNbElements, int &iNbMaxElements);	// Extrait les données d'une balise
	ResourceResult	ConvertTextToArray			(const char* ArrayText, float** Array, int iCount, int iStride);				// Remplit un double tableau de float à l'aide d'un texte
	ResourceResult	ConvertTextToArray			(const char* ArrayText, float*  Array, int iCount);								// Remplit un tableau de float à l'aide d'un texte
	ResourceResult	ConvertTextToArray			(const char* ArrayText, int*    Array, int iCount);								// Remplit un tableau de int à l'aide d'un texte

	ResourceResult	FillVBArray					(TiXmlNode* TrianglesNode,  Vertex *&VertexBuffer, int *&IndexBuffer);			// Remplit le tableau de vertex
	void			FillVertex					(int VertexIndex, int FaceIndex,  Vertex *&VertexBuffer, int *&IndexBuffer);

	ResourceResult	FillSkinArray				(TiXmlNode* VertexWeightsNode);													// Remplit le tableau de skinning

	ResourceResult	FillFacesArray				(TiXmlNode* TrianglesNode);														// Remplit le tableau de faces
};

#endif	// _Mesh_Loader_H