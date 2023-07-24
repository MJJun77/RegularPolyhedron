cmake_minimum_required(VERSION 3.0.0)
project(ShowMainPolyHedron VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(externals/glfw)
add_subdirectory(externals/glm)

#==========================================================
#==========================================================
add_executable(ShowMainPolyHedron 
	main-gl-prog.cpp 
	externals/glad/glad.c
	externals/CombiTraversal/CombiIdxMgr.cpp
	VertexManager.cpp
	FaceManager.cpp
	simple_aux.cpp
	PolyHedronDrawer.cpp
	DrawingObject.cpp

	externals/imgui/imgui.cpp
	externals/imgui/imgui_draw.cpp
	externals/imgui/imgui_tables.cpp
	externals/imgui/imgui_widgets.cpp
	externals/imgui/backends/imgui_Impl_glfw.cpp
	externals/imgui/backends/imgui_Impl_opengl3.cpp
)

target_link_libraries(ShowMainPolyHedron glm)
target_link_libraries(ShowMainPolyHedron glfw)
target_include_directories(ShowMainPolyHedron PRIVATE externals/glm)
target_include_directories(ShowMainPolyHedron PRIVATE externals/glad)
target_include_directories(ShowMainPolyHedron PRIVATE externals/imgui)
target_include_directories(ShowMainPolyHedron PRIVATE externals/)
target_include_directories(ShowMainPolyHedron PRIVATE externals/CombiTraversal)

#==========================================================
#==========================================================
add_executable(Example_ConsoleProgram
	main-example.cpp 
	externals/CombiTraversal/CombiIdxMgr.cpp
	VertexManager.cpp
	FaceManager.cpp
	simple_aux.cpp
)
target_link_libraries(Example_ConsoleProgram glm)
target_include_directories(Example_ConsoleProgram PRIVATE externals/glm)
target_include_directories(Example_ConsoleProgram PRIVATE externals/CombiTraversal)
