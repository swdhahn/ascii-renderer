#include "Model.h"

Model::Model(red::Vector3 *vertices, red::Vector3 *normals, int verticesLength, int normalsLength) :
		vertices(vertices), normals(normals), verticesLength(verticesLength), normalsLength(normalsLength) {
}

Model::Model(red::Vector3 *vertices, red::Vector3 *normals, int *indices, int verticesLength, int normalsLength, int indicesLength) :
		vertices(vertices), normals(normals), indices(indices), verticesLength(verticesLength), indicesLength(indicesLength), normalsLength(normalsLength) {
}

Model::~Model() {
	delete[] vertices;
    delete[] normals;
	delete[] indices;
}

Model* loadOBJIntoModel(const char* path) 
{
	std::cout << "Loading OBJ: " << path << std::endl;
	std::vector<std::string> lines = readFileIntoArray(path);

	std::cout << "Read file" << std::endl;

	std::vector<OBJVertex> vertices;
	std::vector<red::Vector3> normals;
	std::vector<int> indices;
	for(std::string line: lines) {
		std::vector<std::string> nlines = split(line, " ");
		if (line[0] == 'v' && line[1] == ' ') 
		{
			OBJVertex vert = OBJVertex(vertices.size(), red::Vector3(std::atof(nlines[1].c_str()), std::atof(nlines[2].c_str()), std::atof(nlines[3].c_str())));
			vertices.push_back(vert);
		}
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
			red::Vector3 normal = red::Vector3(std::atof(nlines[1].c_str()), std::atof(nlines[2].c_str()), std::atof(nlines[3].c_str()));
			normals.push_back(normal);
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			
			std::vector<std::string> vertex1 = split(nlines[1], "/");
			std::vector<std::string> vertex2 = split(nlines[2], "/");
			std::vector<std::string> vertex3 = split(nlines[3], "/");
			processVertex(vertex1, vertices, indices);
			processVertex(vertex2, vertices, indices);
			processVertex(vertex3, vertices, indices);
		}
	}
	// removing unused verts
	for (OBJVertex& vertex : vertices) {
		if (vertex.m_normalIndex == -1) {
			vertex.m_normalIndex = 0;
		}
	}
	red::Vector3* verticesArray = new red::Vector3[vertices.size()];
	red::Vector3* normalsArray = new red::Vector3[vertices.size()];
	
	// storing data
	for (int i = 0; i < vertices.size(); i++) {
		OBJVertex* currentVertex = &vertices[i];
		verticesArray[i] = currentVertex->m_position;
		normalsArray[i] = normals[currentVertex->m_normalIndex];
		if (currentVertex->m_deletePls)
		{
			delete currentVertex;
		}
	}

	int* indicesArray = new int[indices.size()];
	for (int i = 0; i < indices.size(); i++) {
		indicesArray[i] = indices[i];
	}

	return new Model(verticesArray, normalsArray, indicesArray, vertices.size(), vertices.size(), indices.size());
}

void processVertex(std::vector<std::string>& vertex, std::vector<OBJVertex>& vertices, std::vector<int>& indices)
{
	int index = std::atoi(vertex[0].c_str()) - 1;

	OBJVertex* currentVertex = &vertices[index];
	int normalIndex = std::atoi(vertex[1].c_str()) - 1;
	if ((*currentVertex).m_normalIndex == -1) {
		(*currentVertex).m_normalIndex = (normalIndex);
		indices.push_back(index);
	}
	else {
		dealWithAlreadyProcessedVertex(currentVertex, normalIndex, indices, vertices);
	}
}

void dealWithAlreadyProcessedVertex(OBJVertex* previousVertex, int& newNormalIndex, std::vector<int>& indices, std::vector<OBJVertex>& vertices)
{
	if (previousVertex->m_normalIndex == (newNormalIndex)) {
		indices.push_back((*previousVertex).m_index);
	}
	else {
		if (!previousVertex->m_duplicateIsNull && previousVertex->m_duplicate != nullptr) {
			dealWithAlreadyProcessedVertex(previousVertex->m_duplicate, newNormalIndex, indices, vertices);
		}
		else {
			OBJVertex* duplicateVertex = new OBJVertex(vertices.size(), previousVertex->m_position);
			duplicateVertex->m_deletePls = true;
			duplicateVertex->m_normalIndex = newNormalIndex;
			previousVertex->m_duplicate = duplicateVertex;
			previousVertex->m_duplicateIsNull = false;
			vertices.push_back(*duplicateVertex);
			indices.push_back((*duplicateVertex).m_index);
		}

	}
}