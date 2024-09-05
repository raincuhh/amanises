cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("amanises")

set(CMAKE_CXX_STANDARD 20)

add_executable (amanises "src/driver/main.cpp" "src/frontend/lexer.cpp" "src/utils/logger.cpp" "src/frontend/ast.cpp" "src/frontend/parser.cpp" "src/utils/utils.cpp" "src/driver/process.cpp" "src/error/error.cpp")

set_target_properties(amanises PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)  # For static libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)  # For shared libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})      # For executables

set(CMAKE_OBJECT_PATH ${CMAKE_BINARY_DIR}/out)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET amanises PROPERTY CXX_STANDARD 20)
endif()
