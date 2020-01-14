project(IR_analyzer)

cmake_minimum_required(VERSION 3.12)

file(GLOB SOURCES
    "source/*.h"
    "source/*.cpp"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/AnalyzerSDK/include)
add_library(${PROJECT_NAME} SHARED ${SOURCES}) 
