cmake_minimum_required(VERSION 3.6)
project(unity_native_particles)

if (WIN32)
	include(GenerateExportHeader)
	include_directories("C:/Program Files/Unity 5.5.1f1/Editor/Data/PluginAPI")
	include_directories("src")


	file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.h" "src/*.hpp")

	set(TARGET libnativeparticles)
	add_library(${TARGET} SHARED ${SOURCES})
#	GENERATE_EXPORT_HEADER(libflexiparts
#		BASE_NAME flexiparts
#		EXPORT_MACRO_NAME flexiparts_EXPORT
#		EXPORT_FILE_NAME flexiparts_export.h
#		STATIC_DEFINE flexiparts_BUILD_AS_STATIC
#	)

	set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 14)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET})
	string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}") # no-exceptions
	set(CMAKE_CXX_FLAGS "/GR-") # no rtti
	find_package(OpenGL REQUIRED)
	target_link_libraries(${TARGET} ${OPENGL_LIBRARIES})

	add_custom_command(TARGET ${TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:${TARGET}>
		"${CMAKE_CURRENT_BINARY_DIR}/../unity_project/Assets/Plugins/x86_64/${TARGET}.dll"
	)

endif(WIN32)
