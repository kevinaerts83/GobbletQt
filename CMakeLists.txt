cmake_minimum_required(VERSION 3.16)
project(Gobblet VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(Gobblet WIN32 MACOSX_BUNDLE
    state.h state.cpp
    board.cpp board.h
    board3d.cpp board3d.h
    gobbler.cpp gobbler.h
    gobbler3d.cpp gobbler3d.h
    main.cpp
    matrix.cpp matrix.h
    mediator.cpp mediator.h
    setupBoard.h
    shape.cpp shape.h
    shape3d.cpp shape3d.h
)

add_library(gobblet_ai
    ai.cpp ai.h
    aimove.cpp aimove.h
)

target_include_directories(gobblet_ai PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Gobblet PRIVATE gobblet_ai)

set(RESOURCES qml.qrc)

qt_add_qml_module(Gobblet
    URI Gobbler
    VERSION 1.0
    QML_FILES
        main.qml
        menu.qml
        game.qml
        gobbler.qml
    NO_RESOURCE_TARGET_PATH
    # RESOURCES android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/gradle.properties android/gradlew android/gradlew.bat
)

target_compile_definitions(Gobblet PRIVATE
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(Gobblet PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Quick
)

if(NOT ANDROID)
    install(TARGETS Gobblet
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()
if(ANDROID)
    set_property(TARGET Gobblet APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android")
    set_property(TARGET Gobblet APPEND PROPERTY QT_ANDROID_ABIS "arm64-v8a")
    set_property(TARGET Gobblet APPEND PROPERTY QT_ANDROID_MIN_SDK_VERSION 21)
    set_property(TARGET Gobblet APPEND PROPERTY QT_ANDROID_TARGET_SDK_VERSION 34)
endif()

qt_generate_deploy_qml_app_script(
    TARGET Gobblet
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
    MACOS_BUNDLE_POST_BUILD
)
install(SCRIPT ${deploy_script})

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

find_package(Qt6 REQUIRED COMPONENTS Test)
add_executable(myTest test_ai.cpp)
target_link_libraries(myTest PRIVATE Qt6::Test gobblet_ai)
