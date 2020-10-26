cmake_minimum_required(VERSION 2.8)
project(main_deploy)

IF (UNIX)
	MESSAGE(STATUS "Now is Unix build")
    set(OpenCV_DIR "${PROJECT_SOURCE_DIR}/thirdparty/OpenCV430/ubuntu-16.04/lib/cmake/opencv4/" CACHE PATH "opencv location")
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release )
ELSEIF (WIN32)
	MESSAGE(STATUS "Now is Windows build")
	set(OpenCV_DIR "D:/Software/OpenCV/install/" CACHE PATH "opencv location")	
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release )
ENDIF ()


message (${PROJECT_NAME})
message (${PROJECT_SOURCE_DIR})

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS})
#link_directories(${OpenCV_LIBS_DIR})

#project sources
file (GLOB LEARN_SRC ${PROJECT_SOURCE_DIR}/basic/*.cpp ${PROJECT_SOURCE_DIR}/featrue/*.cpp)
file (GLOB CORE_SRC "*.cpp" "*.h")

set(PROJECT_SRC ${CORE_SRC} ${LEARN_SRC})
add_executable(${PROJECT_NAME} ${PROJECT_SRC})  #STATIC  SHARED
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} -ldl)  
