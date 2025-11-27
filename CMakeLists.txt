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

set_property(GLOBAL PROPERTY QT_ANDROID_APPLICATION_TARGET Gobblet)

# --- Output folders for all targets ---
if(NOT ANDROID)
set_target_properties(gobblet_ai blues Gobblet PROPERTIES
   ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
   LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
endif()

# This dummy target is used to make Qt Creator show additional files
#add_custom_target(extra_files ALL SOURCES
#    android/AndroidManifest.xml
#    android/build.gradle
#    android/gradle.properties
#    android/gradle/wrapper/gradle-wrapper.jar
#    android/gradle/wrapper/gradle-wrapper.properties
#    android/gradlew
#    android/gradlew.bat
#    android/res/values/libs.xml
#    android/res/xml/qtprovider_paths.xml
#)
