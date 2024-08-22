# CMakeList.txt : CMake project for QuickLookup, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("QuickLookup")

option(ALIGN_CENTER "Enable center alignment for search results" OFF)
if(ALIGN_CENTER)
	 add_definitions(-DALIGN_CENTER)
endif()

option(GETCH_INPUT "Enable using getch() for search bar input i.e. refreshing search results after each character" OFF)
if(GETCH_INPUT)
	 add_definitions(-DGETCH_INPUT)
endif()

# Add source to this project's executable.
add_executable (QuickLookupExample  "QuickLookup.h" "Example.cpp" "ClipboardWin.h" "ClipboardLin.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET QuickLookupExample PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
