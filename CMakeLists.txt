cmake_minimum_required(VERSION 3.0.0)
project(Engine VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
	main.cpp
)

add_subdirectory(${ENGINE_SOURCE_DIR}/modules/vkmGL)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(${ENGINE_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME}
	vkmGL
)
