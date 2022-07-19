cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION ${CMAKE_VERSION})
set (EMBRYO_ROOT ${CMAKE_CURRENT_LIST_DIR})
project(Embryo_classifier VERSION "1.0")
include(utils.cmake)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Copyright CVFn 2019")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(cmake/SetBoostEnv.cmake)
include(cmake/SetNlohmannEnv.cmake)
include(cmake/SetONNXRuntime.cmake)
include(cmake/CopyFilesMacros.cmake)
include(cmake/CopyDLLsForEmbryoProjectMacro.cmake)


set(CVStudio_include ${CMAKE_CURRENT_LIST_DIR}/CVStudio_include)
set(CVStudio_lib ${CMAKE_CURRENT_LIST_DIR}/CVStudio_lib)

if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
endif()

message(STATUS "-------")
message(STATUS ${CVStudio_include})
message(STATUS "-------")

add_subdirectory(CVStudio_sources/FishClassifier_lib)
add_subdirectory(CVStudio_sources/FishLabeler_gui_lib)

add_subdirectory(CVStudio_sources/FishLabeler)
add_subdirectory(CVStudio_sources/FishClassifier_gui) 
