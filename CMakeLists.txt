cmake_minimum_required(VERSION 3.21.0)
project(Engine VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
	main.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/modules/minitrace)
add_subdirectory(${PROJECT_SOURCE_DIR}/modules/nlohmann)
add_subdirectory(${PROJECT_SOURCE_DIR}/modules/vkmGL)

add_executable(${PROJECT_NAME} ${SOURCES})

# Include the sources from subdirectories
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

# Link the main target with external libraries
target_link_libraries(${PROJECT_NAME}
	minitrace
	nlohmannjson
	vkmGL
	glfw
	imgui
	${CMAKE_DL_LIBS}
)
