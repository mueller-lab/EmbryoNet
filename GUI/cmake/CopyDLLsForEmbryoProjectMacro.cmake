# Copy Dlls for all dependencies for debug and release configurations
macro(copyDllsForEmbryoClassifier destDir)

	set(QT_DLL_DIR_RELEASE $ENV{QT_DLL_DIR_RELEASE})

    set(ONNXRUNTIME_DIR $ENV{ONNXRUNTIME_DIR})
    set(OPENCV_DLL_DIR_RELEASE $ENV{OPENCV_DLL_DIR_RELEASE})
	set(ZEBRAFISH_MODEL_DIR ${CMAKE_CURRENT_LIST_DIR}/../../Default_Models/ZebraFish)

	set(ZEBRAFISH_MODEL_DIR_CPU ${ZEBRAFISH_MODEL_DIR}/CPU)
	set(ZEBRAFISH_MODEL_DIR_GPU ${ZEBRAFISH_MODEL_DIR}/GPU)
	
	file(GLOB ZEBRAFISH_MODEL_FILES_CPU ${ZEBRAFISH_MODEL_DIR_CPU}/*)
	file(GLOB ZEBRAFISH_MODEL_FILES_GPU ${ZEBRAFISH_MODEL_DIR_GPU}/*)

    message(STATUS "Configuring files for directory ${destDir}")
    make_directory(${destDir}/Release/CPU)
	make_directory(${destDir}/Release/GPU)
		
	# List all DDLs from all dependencies
    # Add here new dependencies... 	
	file(GLOB OPENCVDLLs_Release ${OPENCV_DLL_DIR_RELEASE}/*.dll)
		
	set(QTDLLS ${QT_DLL_DIR_RELEASE}/Qt5Gui.dll ${QT_DLL_DIR_RELEASE}/Qt5Core.dll  ${QT_DLL_DIR_RELEASE}/Qt5Widgets.dll)
	
	foreach(OPENCV_Dll_Release ${OPENCVDLLs_Release})
        file(COPY ${OPENCV_Dll_Release} DESTINATION ${destDir}/CPU/Release/)
		file(COPY ${OPENCV_Dll_Release} DESTINATION ${destDir}/GPU/Release/)
    endforeach(OPENCV_Dll_Release)
	
	foreach(QTDLL ${QTDLLS})
        file(COPY ${QTDLL} DESTINATION ${destDir}/CPU/Release/)
		file(COPY ${QTDLL} DESTINATION ${destDir}/GPU/Release/)
    endforeach(QTDLL)
	
		
	file(GLOB QT_PLATFORMS_DLLS ${QT_DLL_DIR_RELEASE}/../../plugins/platforms/*.dll)
	file(GLOB QT_IMAGEFORMATS_DLLS ${QT_DLL_DIR_RELEASE}/../../plugins/imageformats/*.dll)

	make_directory(${destDir}/CPU/Release/imageformats/)
	make_directory(${destDir}/GPU/Release/imageformats/)
	make_directory(${destDir}/CPU/Release/platforms/)
	make_directory(${destDir}/GPU/Release/platforms/)
	
	foreach(QT_IMAGEFORMATS_DLL ${QT_IMAGEFORMATS_DLLS})
        file(COPY ${QT_IMAGEFORMATS_DLLS} DESTINATION ${destDir}/CPU/Release/imageformats)
		file(COPY ${QT_IMAGEFORMATS_DLLS} DESTINATION ${destDir}/GPU/Release/imageformats)
    endforeach(QT_IMAGEFORMATS_DLL)
	
	foreach(QT_PLATFORMS_DLL ${QT_PLATFORMS_DLLS})
        file(COPY ${QT_PLATFORMS_DLL} DESTINATION ${destDir}/CPU/Release/platforms)
		file(COPY ${QT_PLATFORMS_DLL} DESTINATION ${destDir}/GPU/Release/platforms)
    endforeach(QT_PLATFORMS_DLL)
	
	
	file(GLOB CUDA_DLLs ${ONNXRUNTIME_DIR}/cuda/*.dll)
	file(GLOB ONNXRUNTIME_DLLS ${ONNXRUNTIME_DIR}/*.dll)
	
	
	foreach(ONNXRUNTIME_DLL ${ONNXRUNTIME_DLLS})
        file(COPY ${ONNXRUNTIME_DLL} DESTINATION ${destDir}/GPU/Release)
    endforeach(ONNXRUNTIME_DLL)

	foreach(CUDA_DLL ${CUDA_DLLs})
		file(COPY ${CUDA_DLL} DESTINATION ${destDir}/GPU/Release)		
    endforeach(CUDA_DLL)
	
	foreach(ZEBRAFISH_MODEL_FILE_CPU ${ZEBRAFISH_MODEL_FILES_CPU})
        file(COPY ${ZEBRAFISH_MODEL_FILE_CPU} DESTINATION ${destDir}/CPU/Release)	
    endforeach(ZEBRAFISH_MODEL_FILE_CPU)
	
	foreach(ZEBRAFISH_MODEL_FILE_GPU ${ZEBRAFISH_MODEL_FILES_GPU})
        file(COPY ${ZEBRAFISH_MODEL_FILE_GPU} DESTINATION ${destDir}/GPU/Release)	
    endforeach(ZEBRAFISH_MODEL_FILE_GPU)
	
	


endmacro(copyDllsForTools)


# Copy Dlls for all dependencies for debug and release configurations
macro(copyDllsForEmbryoLabeler destDir)


	set(QT_DLL_DIR_DEBUG $ENV{QT_DLL_DIR_DEBUG})
	set(QT_DLL_DIR_RELEASE $ENV{QT_DLL_DIR_RELEASE})
	
    set(ONNXRUNTIME_DIR $ENV{ONNXRUNTIME_DIR})
    set(OPENCV_DLL_DIR_RELEASE $ENV{OPENCV_DLL_DIR_RELEASE})
	set(OPENCV_DLL_DIR_DEBUG $ENV{OPENCV_DLL_DIR_DEBUG})

	
	
	set(LABELER_EXTRA_FILES_DIR ${CMAKE_CURRENT_LIST_DIR}/../../Default_Models/Labeler_files)
    file(GLOB LABELER_EXTRA_FILES ${LABELER_EXTRA_FILES_DIR}/*)
	foreach(LABELER_EXTRA_FILE ${LABELER_EXTRA_FILES})
        file(COPY ${LABELER_EXTRA_FILE} DESTINATION ${destDir}/Release)	
		file(COPY ${LABELER_EXTRA_FILE} DESTINATION ${destDir}/Debug)	
    endforeach(LABELER_EXTRA_FILE)





    message(STATUS "Configuring files for directory ${destDir}")
    make_directory(${destDir}/Release)
	make_directory(${destDir}/Debug)
		
	# List all DDLs from all dependencies
    # Add here new dependencies... 	
    file(GLOB OPENCVDLLs_Debug ${OPENCV_DLL_DIR_DEBUG}/*.dll)
	file(GLOB OPENCVDLLs_Release ${OPENCV_DLL_DIR_RELEASE}/*.dll)
	
	# Copy different DLLs to Debug and Release
    foreach(OPENCV_Dll_Debug ${OPENCVDLLs_Debug})
    		file(COPY ${OPENCV_Dll_Debug} DESTINATION ${destDir}/Debug)
    endforeach(OPENCV_Dll_Debug)
	
	foreach(OPENCV_Dll_Release ${OPENCVDLLs_Release})
        file(COPY ${OPENCV_Dll_Release} DESTINATION ${destDir}/Release)
    endforeach(OPENCV_Dll_Release)
	
	file(GLOB QT_PLATFORMS_DLLS ${QT_DLL_DIR_DEBUG}/../../plugins/platforms/*.dll)
	file(GLOB QT_IMAGEFORMATS_DLLS ${QT_DLL_DIR_DEBUG}/../../plugins/imageformats/*.dll)

	make_directory(${destDir}/Release/imageformats/)
	make_directory(${destDir}/Release/platforms/)
	make_directory(${destDir}/Debug/imageformats/)
	make_directory(${destDir}/Debug/platforms/)
	
	
	foreach(QT_IMAGEFORMATS_DLL ${QT_IMAGEFORMATS_DLLS})
        file(COPY ${QT_IMAGEFORMATS_DLLS} DESTINATION ${destDir}/Release/imageformats)
		file(COPY ${QT_IMAGEFORMATS_DLLS} DESTINATION ${destDir}/Debug/imageformats)
    endforeach(QT_IMAGEFORMATS_DLL)
	
	foreach(QT_PLATFORMS_DLL ${QT_PLATFORMS_DLLS})
        file(COPY ${QT_PLATFORMS_DLL} DESTINATION ${destDir}/Release/platforms)
		file(COPY ${QT_PLATFORMS_DLL} DESTINATION ${destDir}/Debug/platforms)
    endforeach(QT_PLATFORMS_DLL)
	
	set(QTDLLS_RELEASE ${QT_DLL_DIR_RELEASE}/Qt5Gui.dll ${QT_DLL_DIR_RELEASE}/Qt5Core.dll  ${QT_DLL_DIR_RELEASE}/Qt5Widgets.dll)
	set(QTDLLS_DEBUG ${QT_DLL_DIR_DEBUG}/Qt5Guid.dll ${QT_DLL_DIR_DEBUG}/Qt5Cored.dll  ${QT_DLL_DIR_DEBUG}/Qt5Widgetsd.dll)

    foreach(QTDLL ${QTDLLS_RELEASE})
        file(COPY ${QTDLL} DESTINATION ${destDir}/Release)
    endforeach(QTDLL)
	
	foreach(QTDLL ${QTDLLS_DEBUG})
        file(COPY ${QTDLL} DESTINATION ${destDir}/Debug)
    endforeach(QTDLL)
	
	
endmacro(copyDllsForTools)
 