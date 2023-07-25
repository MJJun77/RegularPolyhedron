// For OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// For glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Util from Joey LearnOpenGL
#include "externals/learnOpenGL/Camera.h"

// For my code
#include <iostream>
#include <string>
#include <sstream>
#include "VertexManager.h"
#include "FaceManager.h"
#include "PolyHedronDrawer.h"

// For imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void CreateImguiUI(GLFWwindow* window, const char* glslVersion = NULL);
void UpdateImguiUI();

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

#define DEF_CAM_POS (glm::vec3(0.0f, 0.0f, 6.0f))
#define DEF_CAM_UP  (glm::vec3(0.0f, 1.0f, 0.0f))
Camera myCam(DEF_CAM_POS, DEF_CAM_UP);

float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// Global Setting - Adjustible by imgui
int giMaxStepSetting = 50; // Unit is 100, so real val is 50 * 100
int giPolyHedronVertexCnt = 12;
float gfDoneCheckDiffSetting = 0.1f; // Imgui min resolution is 0.001 ...
float gfRepulsivePower = 0.05f;
float gfReservedStepMSec = 30.0f;
bool gbRestartClicked = false;

// Main while loop state variable
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int giLoopStep = 0;
bool gbStabledNotified = false;
string gStrLatestMsg;

void InitMainLoopState()
{
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    giLoopStep = 0;
    gbStabledNotified = false;
    gStrLatestMsg = "";
}

// imgui max resolution is 0.001, so ...
template <typename T>
inline T AdjustScaleSetting(T fVal, T factor)
{
    return fVal * factor;
}

void InitObjects(VertexManager* pVertexMgr, FaceManager* pFaceMgr, PolyHedronDrawer* pPolyDrawer, Camera* pCam)
{
    pCam->InitStatus(DEF_CAM_POS, DEF_CAM_UP);

    pVertexMgr->Init(AdjustScaleSetting(gfDoneCheckDiffSetting, 0.001f));
    pVertexMgr->AddRandomVertices(giPolyHedronVertexCnt);
    pFaceMgr->GenerateFace(pVertexMgr->GetVecVertices());

    pPolyDrawer->InitDrawer();
    pPolyDrawer->UpdateFaceData(pFaceMgr->GetDataInfo());
    pPolyDrawer->UpdateViewData(&myCam, 0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Regular Polyhedron Simulation", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);    Disabled mouse moving.  Not necessary for this program.

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    CreateImguiUI(window, nullptr);

    //====================================================
	VertexManager vertexMgr(AdjustScaleSetting(gfDoneCheckDiffSetting, 0.001f));

	FaceManager faceMgr;
	// faceMgr.GenerateFace(vertexMgr.GetVecVertices());

	PolyHedronDrawer polyDrawer;
	// polyDrawer.UpdateFaceData(faceMgr.GetDataInfo());
	// polyDrawer.UpdateViewData(&myCam, 0);

    // render loop
    // -----------

    InitObjects(&vertexMgr, &faceMgr, &polyDrawer, &myCam);
    InitMainLoopState();

    gStrLatestMsg = "On progress...";
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        if (gbRestartClicked)
        {
            gbRestartClicked = false;
            InitObjects(&vertexMgr, &faceMgr, &polyDrawer, &myCam);
            InitMainLoopState();
            gStrLatestMsg = "Restarted. On Progress...";
            continue;
        }

        float currentFrame = static_cast<float>(glfwGetTime()); // Cur value in SEC as float, including msec in fraction.
        currentFrame *= 1000.0f;    // Change it to msec.
        deltaTime = currentFrame - lastFrame; // My laptop, if print, avg is 0.25f in sec -> 250 msec

		if (deltaTime > gfReservedStepMSec)
		{
            lastFrame = currentFrame;
            if (vertexMgr.IsStabled())
            {
                if (!gbStabledNotified)
                {
                    gbStabledNotified = true;

                    ostringstream oss;
                    oss << "Vertex arrangement is stabled at step of: " 
                        << giLoopStep << " / " << AdjustScaleSetting(giMaxStepSetting, 100) << endl;
                    gStrLatestMsg = oss.str();
                }
            }
			else if (giLoopStep < AdjustScaleSetting(giMaxStepSetting, 100))
			{
				giLoopStep++;
                vertexMgr.Step(1, gfRepulsivePower, giLoopStep);
				faceMgr.GenerateFace(vertexMgr.GetVecVertices());
				polyDrawer.UpdateFaceData(faceMgr.GetDataInfo());
			}
            else
            {
                ostringstream oss;
                oss << "Vertex arrangement step is exhausted: " 
                    << AdjustScaleSetting(giMaxStepSetting, 100) << " / " << AdjustScaleSetting(giMaxStepSetting, 100) << endl;
                gStrLatestMsg = oss.str();
            }
		}
        polyDrawer.UpdateViewData(&myCam, 1);
	
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        polyDrawer.Render();

        UpdateImguiUI();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        myCam.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        myCam.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        myCam.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        myCam.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        myCam.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        myCam.ProcessKeyboard(UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        myCam.ProcessKeyboard(ROLL_UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        myCam.ProcessKeyboard(ROLL_DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        myCam.ProcessKeyboard(ROTATE_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        myCam.ProcessKeyboard(ROTATE_RIGHT, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{ // Now, this is deactivated.
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    myCam.ProcessMouseMovement(xoffset, yoffset);
}

//===========================================================================
#include <fstream>
inline bool CheckFileExist(const std::string& name)
{
    ifstream f(name.c_str());
    return f.good();
}

void CreateImguiUI(GLFWwindow* window, const char* glslVersion)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	bool success = ImGui_ImplGlfw_InitForOpenGL(window, true);
	if (success == false)
	{
		printf("ImGui_ImplGlfw_InitForOpenGL failed\n");
	}

	success = ImGui_ImplOpenGL3_Init(glslVersion);
	if (success == false)
	{
		printf("ImGui_ImplOpenGL3_Init failed\n");
	}

	// Search for font file
	const char* fontPath1 = "../resources/fonts/droid_sans.ttf";
	const char* fontPath2 = "../../resources/fonts/droid_sans.ttf";
	const char* fontPath = nullptr;

    bool bExistFontFile1 = CheckFileExist(fontPath1);
    bool bExistFontFile2 = CheckFileExist(fontPath2);
    if (bExistFontFile1)
        fontPath = fontPath1;
    else if (bExistFontFile2)
        fontPath = fontPath2;

	if (fontPath)
	{
        float s_displayScale = 1.0f;
		ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath, 14.0f * s_displayScale);
	}
}

//===========================================================================
void UpdateImguiUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(450, 300), ImGuiCond_FirstUseEver);

    const float widgetSize = 200.0f;
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_None);

    if (ImGui::BeginTabBar("ControlTabs", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Controls"))
        {
            ImGui::SetNextItemWidth(widgetSize);
            ImGui::SliderInt("Vertex Count (12)                ", &giPolyHedronVertexCnt, 4, 50);
            ImGui::SetNextItemWidth(widgetSize);
            ImGui::SliderInt("Max Simul step * 100 (5)         ", &giMaxStepSetting, 1, 50);

            ImGui::SetNextItemWidth(widgetSize);
            ImGui::SliderFloat("Repuls strength (0.5)          ", &gfRepulsivePower, 0.001f, 1.0f);
            ImGui::SetNextItemWidth(widgetSize);
            ImGui::SliderFloat("Stable diff * 10^-3 (0.1)      ", &gfDoneCheckDiffSetting, 0.02f, 1.0f);
            ImGui::SetNextItemWidth(widgetSize);
            ImGui::SliderFloat("Step msec reserve (30.0)       ", &gfReservedStepMSec, 10.0f, 1000.0f);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    if (ImGui::Button("Restart"))
        gbRestartClicked = true;

    ImGui::SetNextItemWidth(widgetSize);
    ImGui::SliderFloat("Moving adjust      ", &myCam.mfUserSpeedSetting, 0.1f, 10.0f);

    ImGui::Text(gStrLatestMsg.c_str());

	ImGui::End();
}
