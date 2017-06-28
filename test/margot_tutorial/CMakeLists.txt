# check the minimum version
cmake_minimum_required( VERSION 2.8.12 )

# the project name
project( TUTORIAL )

################################
#### General configure section
################################

# force the Release build if not already set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set (AGXX_FLAGS "-p /home/andrea/margot_second/tutorial/src --keep_woven --data_joinpoints --builtin_operators")

# setting common c++ flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${AGXX_FLAGS} --Xcompiler -std=c++0x -static-libgcc -Wl,--hash-style=both,--as-needed -pthread -fopenmp -ftemplate-depth=1100" )

# setting debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g3 -O0 -ftemplate-depth=1100")

# setting release with debug info flags
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -march=native -mtune=native -g3 -O2 -ftemplate-depth=1100")

# setting release flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native -O3 -ftemplate-depth=1100")

# check if we need to activate the validation of the results
option( VALIDATION "Validate the results on the error" OFF)
if (VALIDATION)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVALIDATION_ERROR_MARGOT")
endif (VALIDATION)

###############################################
##             EXTERNAL LIB                  ##
###############################################

# add the path to the modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/margot_heel_if/cmake")

# find the required packages
find_package(MARGOT)
find_package(HEEL)


# add the required include directories
include_directories(${MARGOT_INCLUDES} ${HEEL_INCLUDES})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

###############################################
##             CONFIG FILES                  ##
###############################################
#used to generate the .clang_complete file that is used in vim/atom plugins for autocomplete. 
#can be safely removed if not needed.
#configure_file(
#  "${CMAKE_CURRENT_SOURCE_DIR}/config/clang_autocomplete.in"
#  "${PROJECT_SOURCE_DIR}/.clang_complete"
#  @ONLY
#  )
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

###############################################
##             BUILD SECTION                 ##
###############################################

# Set the application name
set( APP_NAME "tutorial")

# Build probability executable
set(SOURCE_FILES src/main.cpp)

add_executable(${APP_NAME} ${SOURCE_FILES})
target_link_libraries(${APP_NAME} ${HEEL_LIBRARIES} ${MARGOT_LIBRARIES})


if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (
      CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}"
      CACHE PATH "default install path"
      FORCE )
endif()

install( TARGETS ${APP_NAME} DESTINATION bin )
