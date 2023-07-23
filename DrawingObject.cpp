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
	mpShader = new Shader("../../resources/shaders/ObjShader_face.vs", "../../resources/shaders/ObjShader_face.fs");
	if (!mpShader->IsNormalStatus())
		return false;

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(maVertexColor), maVertexColor, GL_DYNAMIC_DRAW);

	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

// uniformKey is : `project` or `view`
void GLRenderTriangles::UpdateMat4Info(const char* uniformKey, const glm::mat4& matInfo)
{
	if (mpShader == nullptr)
		return;

	mpShader->use();
	mpShader->setMat4(uniformKey, matInfo);
}

bool GLRenderTriangles::UpdateFaceInfo(const std::vector<FaceInfo>* pvFaces)
{
	mVertexCnt = 0;
	for (int i = 0; i < pvFaces->size(); i++)
	{
		bool bRes1 = AddVertex(pvFaces->at(i).maFaceVertex[0], FACE_COLOR);
		bool bRes2 = AddVertex(pvFaces->at(i).maFaceVertex[1], FACE_COLOR);
		bool bRes3 = AddVertex(pvFaces->at(i).maFaceVertex[2], FACE_COLOR);

		if (!bRes1 || !bRes2 || !bRes3)
			return false;
	}

	return true;
}

bool GLRenderTriangles::AddVertex(const glm::vec3& pos, const glm::vec3& color)
{
	if (mVertexCnt >= eMaxVertexCnt)
		return false;

	maVertexPos[mVertexCnt] = pos;
	maVertexColor[mVertexCnt++] = color;
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
	glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexCnt * sizeof(glm::vec3), maVertexColor);

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
	mpShader = new Shader("../../resources/shaders/ObjShader_line.vs", "../../resources/shaders/ObjShader_line.fs");
	if (!mpShader->IsNormalStatus())
		return false;

	glLineWidth(LINE_WIDTH);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(maVertexColor), maVertexColor, GL_DYNAMIC_DRAW);

	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

// uniformKey is : `project` or `view`
void GLRenderLines::UpdateMat4Info(const char* uniformKey, const glm::mat4& matInfo)
{
	if (mpShader == nullptr)
		return;

	mpShader->use();
	mpShader->setMat4(uniformKey, matInfo);
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
			bool bRes1 = AddVertex(startPos, LINE_COLOR);
			bool bRes2 = AddVertex(endPos, LINE_COLOR);
			startPos = endPos;

			if (!bRes1 || !bRes2)
				return false;
		}
	}

	return true;
}

bool GLRenderLines::AddVertex(const glm::vec3& pos, const glm::vec3& color)
{
	if (mVertexCnt >= eMaxVertexCnt)
		return false;

	maVertexPos[mVertexCnt] = pos;
	maVertexColor[mVertexCnt++] = color;
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

	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexCnt * sizeof(glm::vec3), maVertexColor);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_LINES, 0, mVertexCnt);
	glDisable(GL_BLEND);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
