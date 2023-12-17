#=======================================================================================================================
#Copyright
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================
file(GLOB FISHLABELER_SOURCES include/*.h src/*.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/rc/FishLabeler.rc ${CMAKE_CURRENT_SOURCE_DIR}/rc/FishLabeler_version.rc  )

set(FISH_LABELER_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
copyDllsForEmbryoLabeler(${FISH_LABELER_BUILD_DIR})

find_package(OpenCV REQUIRED)

set(Target FishLabeler)
set(Target_include ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED Core Gui Widgets)

if (WIN32)
add_executable(${Target} WIN32 ${FISHLABELER_SOURCES})
else()
add_executable(${Target}  ${FISHLABELER_SOURCES}  )
endif()

target_include_directories(${Target} PRIVATE ${FishLabeler_gui_INCLUDE}  ${Target_include})

target_link_libraries(${Target} ${FishLabeler_gui_LIB})

target_include_directories(${Target} PUBLIC ${CVStudio_include}/CVUtils/include)
target_link_libraries(${Target} ${CVStudio_lib}/CVUtils.lib)




