#if (NOT ${CMAKE_VERSION} VERSION_LESS 3.1)
#	set(CMAKE_CXX_STANDARD 11)
#else()
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#endif()

# Common definitions for all sub-targets
add_definitions(-D PTW32_STATIC_LIB)	# use pthread static library
if(MSVC)
	# we need this because we are cross-compiling
	add_definitions(-D _CRT_SECURE_NO_WARNINGS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP ")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /Z7")
	#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd" CACHE STRING "" FORCE)
	#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} /MT" CACHE STRING "" FORCE)
	else()
	if (MINGW)
		# we need sizeof(time_t) == 8
		add_definitions(-D __MINGW_USE_VC2005_COMPAT)
	endif (MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Wextra")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fnon-call-exceptions")
endif()

if (BUILD_SHARED_LIBS)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Use project folders (MSVS)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 11)

if ( CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x")
endif()

macro(configure_files srcDir destDir ext)
    message(STATUS "Configuring files for directory ${destDir}")
    make_directory(${destDir})
    file(GLOB templateFiles ${srcDir}/*${ext})
    foreach(templateFile ${templateFiles})
        file(COPY ${templateFile} DESTINATION ${destDir})
    endforeach(templateFile)
endmacro(configure_files)


macro (configure_directory srcDir destDir)
	message(STATUS "Configuring directory ${destDir}")
	if (NOT EXISTS "${destDir}")
		make_directory(${destDir})
	endif()
	file(GLOB  templateFiles ${srcDir})
    foreach(templateFile ${templateFiles})
        file(COPY ${templateFile} DESTINATION ${destDir})
    endforeach(templateFile)
endmacro(configure_directory)


