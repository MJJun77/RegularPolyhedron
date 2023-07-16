cmake_minimum_required(VERSION 3.0.0)
project(ShowMainPolyHedron VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(externals/glfw)
add_subdirectory(externals/glm)

add_executable(ShowMainPolyHedron 
	main-prog.cpp 
	externals/glad/glad.c
	externals/CombiTraversal/CombiIdxMgr.cpp
	VertexManager.cpp
	FaceManager.cpp
	simple_aux.cpp
)

target_link_libraries(ShowMainPolyHedron glm)
target_link_libraries(ShowMainPolyHedron glfw)

target_include_directories(ShowMainPolyHedron PRIVATE externals/glm)
target_include_directories(ShowMainPolyHedron PRIVATE externals/glad)
target_include_directories(ShowMainPolyHedron PRIVATE externals/CombiTraversal)

