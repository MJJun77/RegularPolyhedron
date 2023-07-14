#include <limits>
#include <iostream>

#include "VertexManager.h"

using namespace std;

#define POWER_ADJUST_FACTOR (0.5f)
void VertexManager::Step(int step)
{
	if (mvVertices.size() < 2)
		return;

	std::vector<glm::vec3> vRepulsiveForce;
	vRepulsiveForce.reserve(mvVertices.size());

  //------------------------------------------------------------
  // For each Vertex, calc all the force on current timing
  //------------------------------------------------------------
	for (int i = 0; i < mvVertices.size(); i++)
	{
		glm::vec3 curForceSum(0.0f, 0.0f, 0.0f);
		for (int k = 0; k < mvVertices.size(); k++)
		{
			if (i == k) continue;
			float dist = GetNonZeroAssuredValue(glm::length(mvVertices[i] - mvVertices[k]));
			glm::vec3 repulsDir(glm::normalize(mvVertices[i] - mvVertices[k]));
			curForceSum += POWER_ADJUST_FACTOR * repulsDir / (dist * dist));
		}
		vRepulsiveForce[i] = curForceSum;
	}

  //------------------------------------------------------------
  // Reflect the force to all the vertex
  //------------------------------------------------------------
	for (int i = 0; i < mvVertices.size(); i++)
	{
		mvVertices[i] 
	}

}

void VertexManager::RemoveLastVertex()
{
	if (!mvVertices.empty())
		mvVertices.pop_back();
}

int VertexManager::GetNonZeroAssuredValue(float fVal)
{
	int plusValue = (fVal < 0.0f) ? -1 : 1;
	if (plusValue == -1)
		fVal *= -1.0f;

	const int EpsilonFactor = 10.0f;
	return GET_MAX(fVal, FLT_EPSILON * EpsilonFactor) * plusValue;
}