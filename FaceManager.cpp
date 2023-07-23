#include <limits>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include "FaceManager.h"
#include "simple_aux.h"

//=========================================================================================
//=========================================================================================
FaceInfo::FaceInfo()
{
	SetVertices_Final(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
}
FaceInfo::FaceInfo(const std::vector<glm::vec3>* vecVertex)
{
	if ((vecVertex == nullptr) || vecVertex->size() < 3)
		SetVertices_Final(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	else
		SetVertices_Final(vecVertex->at(0), vecVertex->at(1), vecVertex->at(2));
}
FaceInfo::FaceInfo(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
{
	SetVertices_Final(p0, p1, p2);
}
void FaceInfo::SetVertices_Final(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
{
	maFaceVertex[0] = p0;
	maFaceVertex[1] = p1;
	maFaceVertex[2] = p2;
}
void FaceInfo::MakeAsCCW()
{
	if (IsCCW())
		ReverseWind();
}
bool FaceInfo::IsCCW()
{
	return true;	// TBD
}
void FaceInfo::ReverseWind()
{
	glm::vec3 temp = maFaceVertex[2];
	maFaceVertex[2] = maFaceVertex[0];
	maFaceVertex[0] = temp;
}
void FaceInfo::PrintInfo(const char* msg)
{
	std::cout << msg << "[" << std::fixed << std::setw(4) << std::setprecision(3) 
		<< maFaceVertex[0].x << ", " << maFaceVertex[0].y << ", " << maFaceVertex[0].z << "]  ["
		<< maFaceVertex[1].x << ", " << maFaceVertex[1].y << ", " << maFaceVertex[1].z << "]  ["
		<< maFaceVertex[2].x << ", " << maFaceVertex[2].y << ", " << maFaceVertex[2].z << "]"
		<< "=> Each Len : {" 
		<< glm::length(maFaceVertex[0] - maFaceVertex[1]) << ", "
		<< glm::length(maFaceVertex[1] - maFaceVertex[2]) << ", "
		<< glm::length(maFaceVertex[2] - maFaceVertex[0]) << "}"
		<< std::endl;
}


//=========================================================================================
//=========================================================================================
int FaceManager::GenerateFace(const std::vector<glm::vec3>* vVertices, int gen_algo)
{
	mvFaces.clear();
	if (vVertices->size() < 3)
		return 0;

	if (vVertices->size() == 3)
	{
		FaceInfo curFace(vVertices->at(0), vVertices->at(1), vVertices->at(2));

		mvFaces.push_back(curFace);
		return 1;
	}

	if (gen_algo == 0)
		return GenerateFace_BruteForce(vVertices);
	else if (gen_algo == 1)
		return GenerateFace_ContourDetect(vVertices);
	else
		return 0;
}

#include "CombinationGen.h"
int FaceManager::GenerateFace_BruteForce(const std::vector<glm::vec3>* vVertices)
{
	CombinationGen<glm::vec3> combi_gen(vVertices, 3);
	const std::vector<glm::vec3>* pCurCombi = combi_gen.GetNextCombination();

	int dbgCnt = 0;
	int matchedCnt = 0;
	while (pCurCombi)
	{
		dbgCnt++;
		if (IsOutmostFace(vVertices, pCurCombi, dbgCnt))
		{
			mvFaces.push_back(FaceInfo(pCurCombi));
			matchedCnt++;
		}
		pCurCombi = combi_gen.GetNextCombination();
	}

	return (int)mvFaces.size();
}

int FaceManager::GenerateFace_ContourDetect(const std::vector<glm::vec3>* vVertices)
{
	return 0;	// TBD
}

bool FaceManager::IsVertexOneOfFace(const std::vector<glm::vec3>* faceVertices, const glm::vec3& tgtVertex)
{
	if (faceVertices == nullptr)	// guard code
		return false;
	
	for (auto curIt = faceVertices->begin(); curIt != faceVertices->end(); ++curIt)
	{
		if (*curIt == tgtVertex)
			return true;
	}
	return false;
}

bool FaceManager::IsOutmostFace(const std::vector<glm::vec3>* vAllVertices, const std::vector<glm::vec3>* faceVertices, int dbgIdx)
{
	assert((faceVertices != nullptr) && (faceVertices->size() == 3));
	glm::vec3 faceNormal = glm::normalize(
								glm::cross(
									glm::normalize(faceVertices->at(0) - faceVertices->at(1)), 
									glm::normalize(faceVertices->at(2) - faceVertices->at(1))
								)
							);

	float dot_last = 0.0f;
	for (int i = 0; i < vAllVertices->size(); i++)
	{
		if (IsVertexOneOfFace(faceVertices, vAllVertices->at(i)))
		{
			continue;
		}

		// Note : For 2nd param of dot, you may subtract any one of [0], [1], [2], to get the vector from the face to the vertex
		float dot_result = glm::dot(faceNormal, glm::normalize(vAllVertices->at(i) - faceVertices->at(0)));
		if (IsAlmostZero(dot_result))
		{
			continue;
		}
		
		if (dot_last * dot_result < 0.0f)
		{
			return false;
		}

		dot_last = dot_result;
	}

	return true;
}

void FaceManager::PrintInfo(const char* msg)
{
	std::cout << "all faces info(" << mvFaces.size() << ") " << msg << std::endl;
	for (auto curIt = mvFaces.begin(); curIt != mvFaces.end(); ++curIt)
		curIt->PrintInfo();
}

