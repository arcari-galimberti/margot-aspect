cmake_minimum_required(VERSION 3.0)
project(aspect_project)

set (CMAKE_MODULE_PATH
        "${PROJECT_SOURCE_DIR}/cmake")

# Find libraries
find_package(PugiXML REQUIRED)

find_package(Boost COMPONENTS program_options filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/FunctionMonitorGenerator.cpp include/FunctionMonitorGenerator.h src/AspectParser.cpp include/AspectParser.h src/AspectGenerator.cpp include/AspectGenerator.h src/GoalTuner.cpp include/GoalTuner.h src/StateTuner.cpp include/StateTuner.h src/TunerElements.cpp include/TunerElements.h)
add_executable(aspect_project ${SOURCE_FILES})

# Linking
target_link_libraries(aspect_project ${PUGIXML_LIBRARIES})
target_link_libraries(aspect_project ${Boost_LIBRARIES})
