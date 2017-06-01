cmake_minimum_required(VERSION 3.7)
project(aspect_project)

set (CMAKE_MODULE_PATH
        "${PROJECT_SOURCE_DIR}/cmake")

# Find libraries
find_package(PugiXML REQUIRED)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/AdviceGenerator.cpp include/AdviceGenerator.h src/MonitorGenerator.cpp include/MonitorGenerator.h src/AspectParser.cpp include/AspectParser.h)
add_executable(aspect_project ${SOURCE_FILES})

# Linking
target_link_libraries(aspect_project ${PUGIXML_LIBRARIES})