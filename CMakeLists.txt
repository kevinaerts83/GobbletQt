cmake_minimum_required(VERSION 3.16)
project(Test VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Quick)

qt_standard_project_setup()

qt_add_executable(Test WIN32 MACOSX_BUNDLE
    ai.cpp ai.h
    aimove.cpp aimove.h
    board.cpp board.h
    board3d.cpp board3d.h
    bridge.h
    gobbler.cpp gobbler.h
    gobbler3d.cpp gobbler3d.h
    grades.cpp grades.h
    main.cpp
    matrix.cpp matrix.h
    mediator.cpp mediator.h
    setupBoard.h
    setupMenu.h
    shape.cpp shape.h
    shape3d.cpp shape3d.h
    tilecounter.cpp tilecounter.h
)
qt_add_qml_module(Test
    URI Gobbler
    VERSION 1.0
    QML_FILES
        main.qml
        gobbler.qml
        menu.qml
        game.qml
    NO_RESOURCE_TARGET_PATH
)

target_compile_definitions(Test PRIVATE
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(Test PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Quick
)

install(TARGETS Test
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET Test
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
    MACOS_BUNDLE_POST_BUILD
)
install(SCRIPT ${deploy_script})
