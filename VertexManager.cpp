#include <limits>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

#include "VertexManager.h"
#include "simple_aux.h"

#define EPSILON_FACTOR (10.0f)
using namespace std;

#define M_PI (3.14159f)
random_device rDev{};
mt19937 rEngine{rDev()};
uniform_real_distribution<float> rDist{0.0f, 2.0f * M_PI};

void VertexManager::Step(int repeatStep, float pwrFactor)
{
	if (mvVertices.size() < 2)
		return;

	vector<glm::vec3> vRepulsiveForce;
	vRepulsiveForce.resize(mvVertices.size());

	for (int curStep = 0; curStep < repeatStep; curStep++)
	{
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
				curForceSum += ((pwrFactor * repulsDir) / (dist * dist));
			}
			vRepulsiveForce[i] = curForceSum;
		}

	//------------------------------------------------------------
	// Reflect the force to all the vertex
	//------------------------------------------------------------
		for (int i = 0; i < mvVertices.size(); i++)
		{
			mvVertices[i] += vRepulsiveForce[i];
			mvVertices[i] = glm::normalize(mvVertices[i]);
		}
	}
}

bool VertexManager::IsCloseEachOther(const glm::vec3& left, const glm::vec3& right)
{
	return glm::distance(left, right) <= (EPSILON_FACTOR * FLT_EPSILON);
}

bool VertexManager::IsCloseToOthers(const glm::vec3& newVertex)
{
	for (int i = 0; i < mvVertices.size(); i++)
	{
		if (IsCloseEachOther(mvVertices[i], newVertex))
			return true;
	}

	return false;
}

void VertexManager::AddRandomVertices(int vCnt)
{
	for (int i = 0; i < vCnt; i++)
	{
		float unitLen = 1.0f;
		float xVal, yVal, zVal;
		while (true)
		{
			float randTheta = rDist(rEngine) / 2.0f;
			float randPhi = rDist(rEngine);

			zVal = unitLen * cosf(randTheta);
			xVal = unitLen * sinf(randTheta) * cosf(randPhi);
			yVal = unitLen * sinf(randTheta) * sinf(randPhi);

			if (!IsCloseToOthers(glm::normalize(glm::vec3(xVal, yVal, zVal))))
				break;
			
			cout << "[INFO] Oh, too close vertex randomly generated.  Regen..." << endl;
		}

		AddVertex(glm::normalize(glm::vec3(xVal, yVal, zVal)));
	}
}

void VertexManager::RemoveLastVertex()
{
	if (!mvVertices.empty())
		mvVertices.pop_back();
}

float VertexManager::GetNonZeroAssuredValue(float fVal)
{
	int plusValue = (fVal < 0.0f) ? -1 : 1;
	if (plusValue == -1)
		fVal *= -1;


	return GET_MAX(fVal, FLT_EPSILON * EPSILON_FACTOR) * plusValue;
}

void VertexManager::PrintVerticesInfo(const char* msg)
{
	cout << msg << endl;
	for (int i = 0;i < mvVertices.size(); i++)
	{
		cout << i << "th Vertex [" << std::fixed << std::setw(4) << std::setprecision(3) 
			 << mvVertices[i].x << ", " << mvVertices[i].y << ", " << mvVertices[i].z << "]" << endl;
	}
}
