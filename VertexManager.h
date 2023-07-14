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
	void AddVertex(const glm::vec3& vertex) { mvVertices.push_back(vertex); }
	void Step(int step);
	void RemoveLastVertex() { if (!mvVertices.empty()) mvVertices.pop_back(); }
private:
	int GetNonZeroAssuredValue(float fVal);
};

#endif
