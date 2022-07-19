#=======================================================================================================================
#Copyright
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

file(GLOB FISH_CLASSIFIER_SOURCES include/*.h src/*.cpp)

set(FISH_CLASSIFIER_LIB_CPU FishClassifier_lib_CPU)

set(FISH_CLASSIFIER_LIB_GPU FishClassifier_lib_GPU)

set(FISH_CLASSIFIER_LIB_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED Core Gui Widgets OpenGL OpenGLExtensions)
find_package(OpenCV REQUIRED)

find_package(Boost REQUIRED filesystem)
add_library(${FISH_CLASSIFIER_LIB_GPU} ${FISH_CLASSIFIER_SOURCES} ${USES_TERMINAL})
add_library(${FISH_CLASSIFIER_LIB_CPU} ${FISH_CLASSIFIER_SOURCES} ${USES_TERMINAL})

set(QT5_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS}  ${Qt5OpenGLExtensions_INCLUDE_DIRS})
set(QT5_LIBS ${Qt5Gui_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${Qt5OpenGLExtensions_LIBRARIES} )

target_compile_definitions(${FISH_CLASSIFIER_LIB_CPU} PUBLIC WITH_ONNXRUNTIME=0)
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${FISH_CLASSIFIER_LIB_INCLUDE} ${FRAMEBUFFER_INCLUDE} ${THIRDPARTY_INCLUDE_DIRS} ${CVUTILS_INCLUDE} ${QT5_INCLUDES} ${Boost_INCLUDE_DIRS})
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${InferenceEngine_INCLUDE_DIRS})
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${VMSALGORITHMWRAPPER_LIB_INCLUDE} ${OpenCV_INCLUDE_DIRS} )
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${DEEPSORT_INCLUDE} )
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${NN_INCLUDE} ${IE_INCLUDE} ${NETMANAGER_INCLUDE} )
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${CVUTILS_DNN_INCLUDE}  )
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${NLOHMANN_INCLUDE}  )
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${ONNX_RUNTIME_INCLUDE} )
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${CVStudio_include}/DeepSort/include)
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${CVStudio_include}/CVUtils/include)
target_include_directories(${FISH_CLASSIFIER_LIB_CPU} PUBLIC ${CVStudio_include}/NN/include)

target_link_libraries(${FISH_CLASSIFIER_LIB_CPU} ${QT5_LIBS} ${Boost_LIBRARIES} )
target_link_libraries(${FISH_CLASSIFIER_LIB_CPU} ${OpenCV_LIBS})	
target_link_libraries(${FISH_CLASSIFIER_LIB_CPU} ${CVUTILS_DNN_LIB})
target_link_libraries(${FISH_CLASSIFIER_LIB_CPU} ${DEEPSORT_LIB} ${NETMANAGER_LIB} ${NN_LIB})
target_link_libraries(${FISH_CLASSIFIER_LIB_CPU} ${ONNX_RUNTIME_LIB})




target_compile_definitions(${FISH_CLASSIFIER_LIB_GPU} PUBLIC WITH_ONNXRUNTIME=1)
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${FISH_CLASSIFIER_LIB_INCLUDE} ${FRAMEBUFFER_INCLUDE} ${THIRDPARTY_INCLUDE_DIRS} ${CVUTILS_INCLUDE} ${QT5_INCLUDES} ${Boost_INCLUDE_DIRS})
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${InferenceEngine_INCLUDE_DIRS})
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${VMSALGORITHMWRAPPER_LIB_INCLUDE} ${OpenCV_INCLUDE_DIRS} )
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${DEEPSORT_INCLUDE} )
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${NN_INCLUDE} ${IE_INCLUDE} ${NETMANAGER_INCLUDE} )
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${CVUTILS_DNN_INCLUDE}  )
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${NLOHMANN_INCLUDE}  )
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${ONNX_RUNTIME_INCLUDE} )
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${CVStudio_include}/DeepSort/include)
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${CVStudio_include}/CVUtils/include)
target_include_directories(${FISH_CLASSIFIER_LIB_GPU} PUBLIC ${CVStudio_include}/NN/include)

target_link_libraries(${FISH_CLASSIFIER_LIB_GPU} ${QT5_LIBS} ${Boost_LIBRARIES} )
target_link_libraries(${FISH_CLASSIFIER_LIB_GPU} ${OpenCV_LIBS})	
target_link_libraries(${FISH_CLASSIFIER_LIB_GPU} ${CVUTILS_DNN_LIB})
target_link_libraries(${FISH_CLASSIFIER_LIB_GPU} ${DEEPSORT_LIB} ${NETMANAGER_LIB} ${NN_LIB})
target_link_libraries(${FISH_CLASSIFIER_LIB_GPU} ${ONNX_RUNTIME_LIB})





set (FISH_CLASSIFIER_LIB_INCLUDE ${FISH_CLASSIFIER_LIB_INCLUDE} PARENT_SCOPE)
set (FISH_CLASSIFIER_LIB_GPU ${FISH_CLASSIFIER_LIB_GPU} PARENT_SCOPE)
set (FISH_CLASSIFIER_LIB_CPU ${FISH_CLASSIFIER_LIB_CPU} PARENT_SCOPE)





