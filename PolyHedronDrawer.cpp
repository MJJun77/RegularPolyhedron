#include "PolyHedronDrawer.h"
#include "FaceManager.h"
#include "learnOpenGL/Camera.h"

#include <iostream>
using namespace std;

// Default Setting for face related
#define SCALE_FACTOR (glm::vec3(0.5f))
#define DEF_LIGHT_POS (glm::vec3(1.0f))
#define DEF_LIGHT_COLOR (glm::vec3(0.8f))
#define DEF_OBJ_COLOR (glm::vec3(0.4f, 0.3f, 0.8f))
#define DEF_AMBIENT_STRENGTH (0.3f)

// Default Setting for line related
#define DEF_LINE_COLOR (glm::vec3(0.9f, 0.1f, 0.1f))
#define DEF_LINE_WIDTH 2

bool PolyHedronDrawer::InitDrawer()
{
	const float default_width = 900.0f;
	const float default_height = 600.0f;

	moGlTriangles.Create();
	moGlLines.Create();

	glm::mat4 projection = glm::perspective(glm::radians(30.0f), default_width / default_height, 0.1f, 50.0f);

	moGlTriangles.UpdateMVPMatInfo("projection", projection);
	moGlLines.UpdateMVPMatInfo("projection", projection);

	glm::mat4 modelMat(1.0f);
	modelMat = glm::scale(modelMat, SCALE_FACTOR);
	moGlTriangles.UpdateMVPMatInfo("model", modelMat);
	moGlLines.UpdateMVPMatInfo("model", modelMat);

	moGlTriangles.UpdateLightInfo(DEF_LIGHT_POS, DEF_LIGHT_COLOR, DEF_OBJ_COLOR, DEF_AMBIENT_STRENGTH);
	moGlLines.UpdateLineInfo(DEF_LINE_COLOR, DEF_LINE_WIDTH);

	return true;
}

bool PolyHedronDrawer::UpdateFaceData(const std::vector<FaceInfo>* pvFaces)
{
	moGlTriangles.UpdateFaceInfo(pvFaces);
	moGlLines.UpdateFaceInfo(pvFaces);
	return true;
}

bool PolyHedronDrawer::UpdateViewData(const Camera* pCam, int dbgPath)
{
	glm::mat4 view = pCam->GetViewMatrix();
	moGlTriangles.UpdateMVPMatInfo("view", view);
	moGlLines.UpdateMVPMatInfo("view", view);

	glm::mat4 model = pCam->GetRotateMatrix();
	moGlTriangles.UpdateMVPMatInfo("model", model);
	moGlLines.UpdateMVPMatInfo("model", model);

	return true;
}

bool PolyHedronDrawer::Render()
{
	moGlTriangles.Render();
	moGlLines.Render();

	return true;
}

