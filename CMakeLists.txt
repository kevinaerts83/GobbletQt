cmake_minimum_required(VERSION 3.22)
project(Gobblet VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# --- Find Qt ---
find_package(Qt6 REQUIRED COMPONENTS Core Quick Bluetooth Test)
qt_standard_project_setup()

# --- Add subdirectories ---
add_subdirectory(gobblet_ai)
add_subdirectory(blues)
add_subdirectory(Gobblet)

# --- Output folders for all targets ---
set_target_properties(gobblet_ai blues Gobblet PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(Gobblet PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
)
