cmake_minimum_required(VERSION 3.21.2)
project(nlohmannjson VERSION 0.1.0 LANGUAGES CXX)

# Set the C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message("[BUILDING:NLOHMANN-JSON] Nlohmann-JSON")

add_library(${PROJECT_NAME} STATIC json.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/
)
