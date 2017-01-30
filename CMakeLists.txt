cmake_minimum_required(VERSION 3.5)
project(flexiparts)

if (WIN32)
	include(GenerateExportHeader)
	include_directories("C:/Program Files/Unity 5.5.1f1/Editor/Data/PluginAPI")
	include_directories("src")


	file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

	set(TARGET libflexiparts)
	add_library(${TARGET} SHARED ${SOURCES})
#	GENERATE_EXPORT_HEADER(libflexiparts
#		BASE_NAME flexiparts
#		EXPORT_MACRO_NAME flexiparts_EXPORT
#		EXPORT_FILE_NAME flexiparts_export.h
#		STATIC_DEFINE flexiparts_BUILD_AS_STATIC
#	)

	set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 14)
	add_custom_command(TARGET ${TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:${TARGET}>
		"${CMAKE_CURRENT_BINARY_DIR}/../flexi_parts_unity_project/Assets/Plugins/x86_64/${TARGET}.dll"
	)

endif(WIN32)
