cmake_minimum_required(VERSION 3.21.2)
project(minitrace VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message("[BUILDING:MINITRACE] Mini-Trace")

add_library(${PROJECT_NAME} STATIC minitrace.c)

target_include_directories(${PROJECT_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/
)
