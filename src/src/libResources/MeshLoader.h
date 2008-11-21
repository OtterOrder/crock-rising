#ifndef		_Mesh_Loader_H
#define		_Mesh_Loader_H

//******************************************************************
#include	<string>

#include	<TinyXml/tinyxml.h>
#include	<libCore/Types/Vector.h>

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

	FaceVertex	*m_Faces;

public:

			MeshLoader(void);
	virtual	~MeshLoader(void);

	ResourceResult	Load				(const char *sMeshPath,  Vertex *&VertexBuffer, int *&IndexBuffer, int &iNbVertices, int &iNbIndex);

	ResourceResult	FillArrays			(TiXmlNode* rootNode,  Vertex *&VertexBuffer, int *&IndexBuffer);		// Remplit les tableaux de données
	ResourceResult	ExtractArrayDatas	(TiXmlNode* sourceNode, float** &Array);								// Extrait les données d'une balise
	ResourceResult	ConvertTextToArray	(const char* ArrayText, float** &Array, int iCount, int iStride);		// Rempli un tableau à l'aide d'un texte

	ResourceResult	FillVBArray			(TiXmlNode* TrianglesNode,  Vertex *&VertexBuffer, int *&IndexBuffer);	// Remplit le tableau de vertex
	void			FillVertex			(int VertexIndex, int FaceIndex,  Vertex *&VertexBuffer, int *&IndexBuffer);
	ResourceResult	FillFacesArray		(TiXmlNode* TrianglesNode);												// Remplit le tableau de faces
};

#endif	// _Mesh_Loader_H
