cmake_minimum_required(VERSION 3.0)
project(MarGotAspect)

set (CMAKE_MODULE_PATH
        "${PROJECT_SOURCE_DIR}/cmake")

# Find libraries
find_package(PugiXML REQUIRED)

find_package(Boost COMPONENTS program_options filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/FunctionMonitor.cpp include/FunctionMonitor.h src/AspectParser.cpp include/AspectParser.h src/AspectGenerator.cpp include/AspectGenerator.h src/GoalTuner.cpp include/GoalTuner.h include/RegionMonitor.h src/RegionMonitor.cpp src/StateTuner.cpp include/StateTuner.h src/TunerElements.cpp include/TunerElements.h include/MonitorGenerator.h src/RegionMonitor.cpp)
add_executable(margotaspect ${SOURCE_FILES})

# Linking
target_link_libraries(margotaspect ${PUGIXML_LIBRARIES})
target_link_libraries(margotaspect ${Boost_LIBRARIES})
