#ifndef __POLYHEDRON_DRAWER_HEADER__
#define __POLYHEDRON_DRAWER_HEADER__

#include <vector>
#include "DrawingObject.h"

class FaceInfo;
class Camera;
// #include "FaceManager.h"
// #include "externals/learnOpenGL/Shader.h"
// #include "externals/learnOpenGL/Camera.h"

class PolyHedronDrawer
{
private:
	GLRenderTriangles moGlTriangles;
	GLRenderLines moGlLines;

public:
	PolyHedronDrawer() { InitDrawer(); }
	bool UpdateFaceData(const std::vector<FaceInfo>*);
	bool UpdateViewData(const Camera* pCam, int dbgPath);

	bool Render();
private:
	bool InitDrawer();
};

#endif
