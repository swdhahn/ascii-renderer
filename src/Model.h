#ifndef MODEL
#define MODEL

#include "Maths.h"
#include "Files.h"
#include <string>
#include <vector>

class Model {
public:

    Model(red::Vector3 *vertices, red::Vector3 *normals, int verticesLength, int normalsLength);
    Model(red::Vector3 *vertices, red::Vector3 *normals, int *indices, int verticesLength, int normalsLength, int indicesLength);
	~Model();

    red::Vector3 *vertices = nullptr;
    red::Vector3 *normals = nullptr;
	int *indices = nullptr;
	int verticesLength = -1, normalsLength = -1, indicesLength = -1;

};

class OBJVertex {
public:
	red::Vector3 m_position;
	OBJVertex* m_duplicate = nullptr;
	bool m_duplicateIsNull = true, m_deletePls = false;
	int m_normalIndex = -1, m_index = -1;
	OBJVertex(int index, red::Vector3 position)
		: m_index(index), m_position(position) {}
	OBJVertex(const OBJVertex& v) : m_duplicate(v.m_duplicate), m_normalIndex(v.m_normalIndex), m_index(v.m_index), m_position(v.m_position), m_duplicateIsNull(v.m_duplicateIsNull){}
};
void processVertex(std::vector<std::string>& vertex, std::vector<OBJVertex>& vertices, std::vector<int>& indices);
void dealWithAlreadyProcessedVertex(OBJVertex* previousVertex, int& newTextureIndex, std::vector<int>& indices, std::vector<OBJVertex>& vertices);
Model* loadOBJIntoModel(const char* path);

#endif // MODEL