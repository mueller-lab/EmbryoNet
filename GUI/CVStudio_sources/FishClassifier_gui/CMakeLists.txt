#=======================================================================================================================
#Copyright
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

file(GLOB FISH_CLASSIFIER_GUI_SOURCES include/*.h src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/rc/FishLabeler_CPU.rc ${CMAKE_CURRENT_SOURCE_DIR}/rc/FishClassifier_version.rc)

set(FISH_CLASSIFIER_GUI_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)

set(FISH_CLASSIFIER_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

copyDllsForEmbryoClassifier(${FISH_CLASSIFIER_BUILD_DIR})

find_package(Qt5 REQUIRED Core Gui Widgets)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT5_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS}  ${Qt5OpenGLExtensions_INCLUDE_DIRS})
set(QT5_LIBS ${Qt5Gui_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${Qt5OpenGLExtensions_LIBRARIES} )

######################################################################################
set(FISH_CLASSIFIER_GUI_CPU FishClassifier_gui_CPU)



if (WIN32)
add_executable(${FISH_CLASSIFIER_GUI_CPU} WIN32 ${FISH_CLASSIFIER_GUI_SOURCES} )
else()
add_executable(  ${FISH_CLASSIFIER_GUI_SOURCES}  )
endif()

set_target_properties( ${FISH_CLASSIFIER_GUI_CPU} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CPU )


target_include_directories(${FISH_CLASSIFIER_GUI_CPU} PRIVATE ${FISH_CLASSIFIER_LIB_INCLUDE} ${QT5_INCLUDES} ${FISH_CLASSIFIER_GUI_INCLUDE} ${FishLabeler_gui_INCLUDE})
target_include_directories(${FISH_CLASSIFIER_GUI_CPU} PRIVATE ${CVStudio_include}/AlgorithmBase/include ${CVStudio_include}/CVUtils/include ${CVStudio_include}/CVUtils_dnn/include ${CVStudio_include}/DeepSort/include ${CVStudio_include}/NN/include )
target_link_libraries(${FISH_CLASSIFIER_GUI_CPU} PRIVATE ${FISH_CLASSIFIER_LIB_CPU} ${QT5_LIBS} ${FishLabeler_gui_LIB})
target_link_libraries(${FISH_CLASSIFIER_GUI_CPU} PRIVATE ${CVStudio_lib}/CVUtils.lib ${CVStudio_lib}/cvutils_dnn.lib ${CVStudio_lib}/DeepSort.lib  ${CVStudio_lib}/NN.lib ${CVStudio_lib}/AlgorithmBase.lib)

#target_link_libraries (${FISH_CLASSIFIER_GUI_CPU} PRIVATE ${CVStudio_lib}/CVUtils.lib ${CVStudio_lib}/cvutils_dnn.lib ${CVStudio_lib}/DeepSort.lib  ${CVStudio_lib}/NN.lib ${CVStudio_lib}/AlgorithmBase.lib)

########################################################################################
set(FISH_CLASSIFIER_GUI_GPU FishClassifier_gui_GPU)
file(GLOB FISH_CLASSIFIER_GUI_GPU_SOURCES include/*.h src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/rc/FishLabeler_GPU.rc ${CMAKE_CURRENT_SOURCE_DIR}/rc/FishClassifier_version.rc)

if (WIN32)
add_executable(${FISH_CLASSIFIER_GUI_GPU} WIN32 ${FISH_CLASSIFIER_GUI_GPU_SOURCES} )
else()
add_executable(${FISH_CLASSIFIER_GUI_GPU}  ${FISH_CLASSIFIER_GUI_GPU_SOURCES}  )
endif()

set_target_properties( ${FISH_CLASSIFIER_GUI_GPU} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/GPU )
 
 
target_include_directories(${FISH_CLASSIFIER_GUI_GPU} PRIVATE ${FISH_CLASSIFIER_LIB_INCLUDE} ${QT5_INCLUDES} ${FISH_CLASSIFIER_GUI_INCLUDE} ${FishLabeler_gui_INCLUDE})
target_link_libraries(${FISH_CLASSIFIER_GUI_GPU}  PRIVATE ${FISH_CLASSIFIER_LIB_GPU} ${QT5_LIBS} ${FishLabeler_gui_LIB})
target_link_libraries (${FISH_CLASSIFIER_GUI_GPU} PRIVATE ${CVStudio_lib}/CVUtils.lib ${CVStudio_lib}/cvutils_dnn.lib ${CVStudio_lib}/DeepSort.lib  ${CVStudio_lib}/NN.lib ${CVStudio_lib}/AlgorithmBase.lib)
