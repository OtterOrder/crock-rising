#ifndef		_Mesh_H
#define		_Mesh_H

//******************************************************************
#include	"Resource.h"

#include	<TinyXml/tinyxml.h>

//******************************************************************

class Mesh : public Resource
{
public:

	Mesh(void);
	~Mesh();

	ResourceResult			Load		(crc32 resource);

private:
	int		m_iNbVertices;

	float** m_Positions;
	float** m_Normals;
	float** m_TexCoords;

	LPDIRECT3DDEVICE9		m_pDevice;		//Device utilisé par le renderer

	LPDIRECT3DVERTEXBUFFER9	m_pVB;			// Vertex Buffer de la ressource
	LPDIRECT3DINDEXBUFFER9	m_pIB;			// Index Buffer de la ressource

	ResourceResult	FillArrays	(TiXmlNode* rootNode);			// Remplit les tableaux de données
	ResourceResult	ExtractArrayDatas	(TiXmlNode* sourceNode, float** &Array);		// Extrait les données d'une balise
	ResourceResult	ConvertTextToArray	(char* text, float** Array, int iCount, int iStride);	// Rempli un tableau à l'aide d'un texte
};

//******************************************************************
#endif		// _Mesh_H
