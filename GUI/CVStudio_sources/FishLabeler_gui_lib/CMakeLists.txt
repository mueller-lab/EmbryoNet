#=======================================================================================================================
#Copyright
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================
file(GLOB FISHLABELER_SOURCES include/*.h src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/rc/FishLabeler.rc)

find_package(OpenCV REQUIRED)

set(Target FishLabeler_gui_LIB)
set(Target_include ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED Core Gui Widgets)

add_library(${Target}  ${FISHLABELER_SOURCES}  )

set(QT5_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
set(QT5_LIBS ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES})

target_include_directories(${Target} PUBLIC ${THIRDPARTY_INCLUDE_DIRS} ${CVUTILS_INCLUDE}  ${Target_include}  ${QT5_INCLUDES} )
target_link_libraries(${Target} ${CVUTILS_LIB}  ${OpenCV_LIBS} ${QT5_LIBS})

target_include_directories(${Target} PUBLIC ${NLOHMANN_INCLUDE} ${Boost_INCLUDE_DIRS})
target_link_libraries(${Target} ${Boost_LIBRARIES})

set (FishLabeler_gui_LIB ${Target} PARENT_SCOPE)
set (FishLabeler_gui_INCLUDE ${Target_include} PARENT_SCOPE)

target_include_directories(${Target} PUBLIC ${CVStudio_include}/CVUtils/include)
target_link_libraries(${Target} ${CVStudio_lib}/CVUtils.lib)


