cmake_minimum_required(VERSION 3.0.0)
project(ShowMainPolyHedron VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(externals/glfw)
add_subdirectory(externals/glm)

add_executable(ShowMainPolyHedron 
	main-prog.cpp 
	externals/glad/glad.c
)
target_include_directories(externals/glad)
target_include_directories(externals/learnOpenGL)
target_link_libraries(glm)
target_link_libraries(glfw)