#ifndef __FACE_GENERATOR_HEADER__
#define __FACE_GENERATOR_HEADER__

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FaceInfo
{
public:
	glm::vec3 maFaceVertex[3];
public:
	FaceInfo();
	FaceInfo(const std::vector<glm::vec3>* vecVertex);
	FaceInfo(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
	void MakeAsCCW();
	void PrintInfo(const char* msg = "");
private:
	void SetVertices_Final(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
	void ReverseWind();
	bool IsCCW();
};

class FaceManager
{
private:
	std::vector<FaceInfo> mvFaces;
public:
	FaceManager() {}
	int GenerateFace(const std::vector<glm::vec3>* vVertices, int gen_algo = 0);
	void PrintInfo(const char* msg = "");
	const std::vector<FaceInfo>* GetDataInfo() { return &mvFaces; }
private:
	int GenerateFace_BruteForce(const std::vector<glm::vec3>* vVertices);
	int GenerateFace_ContourDetect(const std::vector<glm::vec3>* vVertices);
	bool IsOutmostFace(const std::vector<glm::vec3>* vAllVertices, const std::vector<glm::vec3>* faceVertices, int dbgIdx);
	bool IsVertexOneOfFace(const std::vector<glm::vec3>* faceVertices, const glm::vec3& tgtVertex);


};

#endif
