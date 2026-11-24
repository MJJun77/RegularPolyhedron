#ifndef __DRAWING_OBJECT_HEADER__
#define __DRAWING_OBJECT_HEADER__

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	// MJ TBD : Is this mandatory ?
#include <glm/gtc/type_ptr.hpp>			// MJ TBD : Is this mandatory ?

class FaceInfo;
class Shader;

//==============================================================
class GLRenderTriangles
{
public:
	GLRenderTriangles() : mVertexCnt(0), mpShader(nullptr), mbInitCreated(false) {}
	bool Create();
	bool UpdateFaceInfo(const std::vector<FaceInfo>* pvFaces);
	void UpdateMVPMatInfo(const char* uniformKey, const glm::mat4& matInfo);
	void UpdateVec3Info(const char* uniformKey, const glm::vec3& vecInfo);
	void UpdateLightInfo(const glm::vec3& litPos, const glm::vec3& litColor, const glm::vec3& objColor, float ambiStr);
	void Render();
private:
	bool AddVertex(const glm::vec3& pos, const glm::vec3& normal);
private:
	int mVertexCnt;
	bool mbInitCreated;
	enum { eVboCnt = 2};
	enum { eMaxVertexCnt = (3 * 512) };

private:	// GL related objects
	unsigned int mVaoId;
	unsigned int mVboIds[eVboCnt];
	unsigned int mProgramId;
	unsigned int mvertexAttr;
	unsigned int mcolorAttrr;
	unsigned int mUniformModel;
	unsigned int mUniformView;
	unsigned int mUniformProj;

private:
	glm::vec3 maVertexPos[eMaxVertexCnt];
	glm::vec3 maVertexNormal[eMaxVertexCnt];
	Shader* mpShader;
};

//==============================================================
class GLRenderLines
{
public:
	GLRenderLines() : mVertexCnt(0), mpShader(nullptr), mbInitCreated(false) {}
	bool Create();
	bool UpdateFaceInfo(const std::vector<FaceInfo>* pvFaces);
	void UpdateMVPMatInfo(const char* uniformKey, const glm::mat4& matInfo);
	void UpdateLineInfo(const glm::vec3& vColor, float lineWidth);
	void Render();
private:
	bool AddVertex(const glm::vec3& pos);
private:
	int mVertexCnt;
	bool mbInitCreated;
	enum { eVboCnt = 1};
	enum { eMaxVertexCnt = (2 * 3 * 512) };

private:	// GL related objects
	unsigned int mVaoId;
	unsigned int mVboIds[eVboCnt];
	unsigned int mProgramId;
	unsigned int mvertexAttr;
	unsigned int mcolorAttrr;
	unsigned int mUniformModel;
	unsigned int mUniformView;
	unsigned int mUniformProj;

private:
	glm::vec3 maVertexPos[eMaxVertexCnt];
	Shader* mpShader;
};

#endif
