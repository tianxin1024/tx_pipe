cmake_minimum_required(VERSION 3.10)

project(tx_pipe LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -w -fdiagnostics-color=always -pthread")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wno-deprecated-declarations)

# OpenCV required
find_package(OpenCV 4.6 REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")


# Gstreamer required
include(FindPkgConfig)
pkg_check_modules(GST   REQUIRED gstreamer-1.0)
pkg_check_modules(GSTAPP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GST_RTSP REQUIRED gstreamer-rtsp-server-1.0)
# message(STATUS "GStreamer library status:")
# message(STATUS "    version: ${GST_VERSION}")
# message(STATUS "    libraries: ${GST_LIBRARIES} ${GSTAPP_LIBRARIES} ${GST_RTSP_LIBRARIES}")
# message(STATUS "    include path: ${GST_INCLUDE_DIRS}")


include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GST_INCLUDE_DIRS})


set(CXX_HEADER_DIRS "")
set(COMMON_LIB_DIRS "")

include_directories(${CXX_HEADER_DIRS})
link_directories(${COMMON_LIB_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)


add_subdirectory(src)
add_subdirectory(example)
