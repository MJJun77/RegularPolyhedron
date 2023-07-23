#include "PolyHedronDrawer.h"
#include "FaceManager.h"
#include "learnOpenGL/Camera.h"

#include <iostream>
using namespace std;

#define SCALE_FACTOR (glm::vec3(0.5f))

bool PolyHedronDrawer::InitDrawer()
{
	const float default_width = 900.0f;
	const float default_height = 600.0f;

	moGlTriangles.Create();
	moGlLines.Create();

	glm::mat4 projection = glm::perspective(glm::radians(30.0f), default_width / default_height, 0.1f, 50.0f);

	moGlTriangles.UpdateMat4Info("projection", projection);
	moGlLines.UpdateMat4Info("projection", projection);

	glm::mat4 modelMat(1.0f);
	modelMat = glm::scale(modelMat, SCALE_FACTOR);
	moGlTriangles.UpdateMat4Info("model", modelMat);
	moGlLines.UpdateMat4Info("model", modelMat);

	cout << "[DBG] Drawer InitDrawer() called" << endl;

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
	moGlTriangles.UpdateMat4Info("view", view);
	moGlLines.UpdateMat4Info("view", view);

	glm::mat4 model = pCam->GetRotateMatrix();
	moGlTriangles.UpdateMat4Info("model", model);
	moGlLines.UpdateMat4Info("model", model);

	return true;
}

bool PolyHedronDrawer::Render()
{
	moGlTriangles.Render();
	moGlLines.Render();

	return true;
}

