#ifndef __VERTEX_MANAGER_HEADER__
#define __VERTEX_MANAGER_HEADER__

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class VertexManager
{
private:
	std::vector<glm::vec3> mvVertices;
public:
	void Clear();
	void Step(int step, float repulsePowerFactor = 1.0f);
	void AddRandomVertices(int cnt);
	void AddVertex(const glm::vec3& vertex) { mvVertices.push_back(vertex); }
	void RemoveLastVertex();
	const std::vector<glm::vec3>* GetVecVertices() { return &mvVertices; }
public:	// Debugging
	void PrintVerticesInfo(const char* msg);

private:
	float GetNonZeroAssuredValue(float fVal);
	bool IsCloseToOthers(const glm::vec3& newVertex);
	bool IsCloseEachOther(const glm::vec3& left, const glm::vec3& right);

};

#endif
