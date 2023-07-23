#include <iostream>

#include "VertexManager.h"
#include "FaceManager.h"
#include "simple_aux.h"

using namespace std;

#define REPULSE_POWER_FACTOR (1.0f)
int main()
{
	VertexManager vertexMgr;
	FaceManager faceMgr;

	vertexMgr.AddRandomVertices(12);
	vertexMgr.PrintVerticesInfo("Initially added");
	faceMgr.GenerateFace(vertexMgr.GetVecVertices());
	faceMgr.PrintInfo();

	vertexMgr.Step(100, REPULSE_POWER_FACTOR);

	vertexMgr.PrintVerticesInfo("After step");
	faceMgr.GenerateFace(vertexMgr.GetVecVertices());
	faceMgr.PrintInfo();

	return 0;
}