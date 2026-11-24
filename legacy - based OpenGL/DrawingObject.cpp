#include "DrawingObject.h"
#include "glad/glad.h"
#include "externals/learnOpenGL/Shader.h"
#include "FaceManager.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))
#define FACE_COLOR (glm::vec3(0.2f, 0.9f, 0.0f))
#define LINE_COLOR (glm::vec3(0.9f, 0.1f, 0.1f))
#define LINE_WIDTH 3

//===================================================================================
//===================================================================================
bool GLRenderTriangles::Create()
{
	if (!mbInitCreated)
	{
		mpShader = new Shader("../../resources/shaders/ObjShader_face.vs", "../../resources/shaders/ObjShader_face.fs");
		if (!mpShader->IsNormalStatus())
			return false;

		mbInitCreated = true;

		glGenVertexArrays(1, &mVaoId);
		glGenBuffers(eVboCnt, mVboIds);

		glBindVertexArray(mVaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(maVertexPos), maVertexPos, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(maVertexNormal), maVertexNormal, GL_DYNAMIC_DRAW);

		// Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return true;
	}
	return true;
}

void GLRenderTriangles::UpdateMVPMatInfo(const char* uniformKey, const glm::mat4& matInfo)
{
	if (mpShader == nullptr)
		return;

	mpShader->use();
	mpShader->setMat4(uniformKey, matInfo);
}

void GLRenderTriangles::UpdateLightInfo(const glm::vec3& litPos, const glm::vec3& litColor, const glm::vec3& objColor, float ambiStr)
{
	UpdateVec3Info("lightPos", litPos);
	UpdateVec3Info("lightColor", litColor);
	UpdateVec3Info("objectColor", objColor);
	mpShader->setFloat("ambientStrength", ambiStr);
}

void GLRenderTriangles::UpdateVec3Info(const char* uniformKey, const glm::vec3& vecInfo)
{
	if (mpShader == nullptr)
		return;

	mpShader->use();
	mpShader->setVec3(uniformKey, vecInfo);
}

bool GLRenderTriangles::UpdateFaceInfo(const std::vector<FaceInfo>* pvFaces)
{
	mVertexCnt = 0;
	for (int i = 0; i < pvFaces->size(); i++)
	{
		FaceInfo curFace(pvFaces->at(i));
		curFace.MakeAsCCW();
		glm::vec3 normVec = curFace.GetNormVec();
		bool bRes1 = AddVertex(curFace.maFaceVertex[0], normVec);
		bool bRes2 = AddVertex(curFace.maFaceVertex[1], normVec);
		bool bRes3 = AddVertex(curFace.maFaceVertex[2], normVec);

		if (!bRes1 || !bRes2 || !bRes3)
			return false;
	}

	return true;
}

bool GLRenderTriangles::AddVertex(const glm::vec3& pos, const glm::vec3& normal)
{
	if (mVertexCnt >= eMaxVertexCnt)
		return false;

	maVertexPos[mVertexCnt] = pos;
	maVertexNormal[mVertexCnt++] = normal;
	return true;
}

void GLRenderTriangles::Render()
{
	if (mVertexCnt == 0)
		return;

	mpShader->use();
	glBindVertexArray(mVaoId);

	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexCnt * sizeof(glm::vec3), maVertexPos);

	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexCnt * sizeof(glm::vec3), maVertexNormal);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, mVertexCnt);
	glDisable(GL_BLEND);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

//===================================================================================
//===================================================================================
bool GLRenderLines::Create()
{
	if (!mbInitCreated)
	{
		mpShader = new Shader("../../resources/shaders/ObjShader_line.vs", "../../resources/shaders/ObjShader_line.fs");
		if (!mpShader->IsNormalStatus())
			return false;

		mbInitCreated = true;

		glGenVertexArrays(1, &mVaoId);
		glGenBuffers(eVboCnt, mVboIds);

		glBindVertexArray(mVaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(maVertexPos), maVertexPos, GL_DYNAMIC_DRAW);

		// Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return true;
	}
	return true;
}

void GLRenderLines::UpdateMVPMatInfo(const char* uniformKey, const glm::mat4& matInfo)
{
	if (mpShader == nullptr)
		return;

	mpShader->use();
	mpShader->setMat4(uniformKey, matInfo);
}

void GLRenderLines::UpdateLineInfo(const glm::vec3& vColor, float lineWidth)
{
	if (mpShader == nullptr)
		return;

	mpShader->use();
	mpShader->setVec3("fsColor", vColor);
	glLineWidth(lineWidth);
}

bool GLRenderLines::UpdateFaceInfo(const std::vector<FaceInfo>* pvFaces)
{
	if (!pvFaces)
		return false;

	mVertexCnt = 0;
	for (auto curIt = pvFaces->begin(); curIt != pvFaces->end(); ++curIt)
	{
		glm::vec3 startPos = curIt->maFaceVertex[2];
		for (int k = 0; k < 3; ++k)
		{
			glm::vec3 endPos = curIt->maFaceVertex[k];
			bool bRes1 = AddVertex(startPos);
			bool bRes2 = AddVertex(endPos);
			startPos = endPos;

			if (!bRes1 || !bRes2)
				return false;
		}
	}

	return true;
}

bool GLRenderLines::AddVertex(const glm::vec3& pos)
{
	if (mVertexCnt >= eMaxVertexCnt)
		return false;

	maVertexPos[mVertexCnt++] = pos;
	return true;
}

void GLRenderLines::Render()
{
	if (mVertexCnt == 0)
		return;

	mpShader->use();
	glBindVertexArray(mVaoId);

	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexCnt * sizeof(glm::vec3), maVertexPos);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_LINES, 0, mVertexCnt);
	glDisable(GL_BLEND);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
