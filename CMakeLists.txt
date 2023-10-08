cmake_minimum_required(VERSION 3.27)

project(fra-airtraffic)

# Because the variety of Linux screensaver implementations and xscreensaver not
# supporting any ui framework apart from X11, Linux support is not a thing yet.
# On Windows, only MSVC is known to work by now. A MinGW build using TDM-GCC had
# issues when no user was logged in:
# https://sourceforge.net/p/tdm-gcc/bugs/324/
if(NOT(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC))
	message(FATAL_ERROR "This project is intended to be built using MSVC.")
endif()

set(C_STANDARD 99)
set(C_STANDARD_REQUIRED ON)
set(C_EXTENSIONS ON)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED OFF)
set(CXX_EXTENSIONS OFF)

# Minimize wxWidgets and set vendor name
include(wxWidgets-features.cmake)

# Use wxWidgets for drawing
add_subdirectory(wxWidgets EXCLUDE_FROM_ALL)

set(CMAKE_EXECUTABLE_SUFFIX .scr)

# Create multiple projects as opposed to multiple configs as contained in the
# original Visual Studio project file. Using add_subdirectory(wxWidgets) would
# create a wxWidgets config for each fra-airtraffic configuration we create here,
# which is not desirable, as we would have to build wxWidgets per config then.
#
# We could use wxWidgets as imported lib, however, add_subdirectory() is simpler.
#
# Targets at top-level are screensavers, having CMAKE_EXECUTABLE_SUFFIX ".scr",
# all targets wanting to build ".exe" files, need to be located in another subdir,
# Using an own CMakeLists.txt. This is due to CMAKE_EXECUTABLE_SUFFIX being a
# directory property, rather than the target's.
#
# Targets:
#
# * fra-airtraffic
#   The screensaver with a debug config to launch with `-s` argument
#
# * fra-airtraffic-preview
#   Preview in the Windows screensaver configuration dialog
#
# * screenshots/screenshots
#   Executable rendering one cycle (day) into screenshot files, later to be
#   combined into animated GIF for the README.

function(add_target TARGET)
	add_executable(${TARGET} WIN32
		${PROJECT_SOURCE_DIR}/container.cpp
		${PROJECT_SOURCE_DIR}/content.cpp
		${PROJECT_SOURCE_DIR}/fra-airtraffic.rc
		${PROJECT_SOURCE_DIR}/gc.c
		${PROJECT_SOURCE_DIR}/json.c
		${PROJECT_SOURCE_DIR}/main.cpp
		${PROJECT_SOURCE_DIR}/preview.cpp
		${PROJECT_SOURCE_DIR}/screenshot.cpp
		${PROJECT_SOURCE_DIR}/stdpaths.cpp
	)

	target_compile_definitions(${TARGET}
		PRIVATE
			_USE_MATH_DEFINES
			WXUSINGDLL
	)

	target_link_libraries(${TARGET}
		PRIVATE
			wx::core
			wx::base
	)

	cmake_parse_arguments(PARSE_ARGV 0 DEBUGGER "" "COMMAND;ARGUMENTS" "")

	set_target_properties(${TARGET} PROPERTIES
		VS_DEBUGGER_COMMAND "${DEBUGGER_COMMAND}"
		VS_DEBUGGER_COMMAND_ARGUMENTS "${DEBUGGER_ARGUMENTS}"
		# We need to find the images for debugging.
		VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
		# We need to find the wxWidgets DLLs for debugging.
		# VS solution/project files are placed in cmake binary dir,
		# where also the wxWidget output is placed.
		VS_DEBUGGER_ENVIRONMENT "PATH=${CMAKE_BINARY_DIR}/wxWidgets/lib/vc_x64_dll;$(PATH)"
	)
endfunction()

add_target(
	fra-airtraffic
		COMMAND "$(TargetPath)"
		ARGUMENTS "-s"
)

add_target(
	fra-airtraffic-preview
		COMMAND "$(Windir)\\System32\\rundll32.exe"
		ARGUMENTS "shell32.dll,Control_RunDLL desk.cpl,fra-airtraffic,@ScreenSaver"
)

add_subdirectory(screenshots)

set_property(DIRECTORY ${CMAKE_CURRENT_LISTS_DIR}
	PROPERTY VS_STARTUP_PROJECT fra-airtraffic
)
