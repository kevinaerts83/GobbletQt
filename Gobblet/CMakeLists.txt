# Gobblet/CMakeLists.txt
qt_add_executable(Gobblet
    #MANUAL_FINALIZATION
    WIN32 MACOSX_BUNDLE
    main.cpp
    state.cpp state.h
    board.cpp board.h
    board3d.cpp board3d.h
    gobbler.cpp gobbler.h
    gobbler3d.cpp gobbler3d.h
    matrix.cpp matrix.h
    mediator.cpp mediator.h
    setupBoard.h
    shape.cpp shape.h
    shape3d.cpp shape3d.h
)

# Link everything together
target_link_libraries(Gobblet
    PRIVATE
        blues
        gobblet_ai
        Qt6::Core
        Qt6::Quick
        Qt6::Bluetooth
)

# QML module setup
qt_add_qml_module(Gobblet
    URI Gobbler
    VERSION 1.0
    QML_FILES
        main.qml
        menu.qml
        game.qml
        gobbler.qml
        help.qml
        scanblues.qml
        BluetoothIcon.qml
    RESOURCES qml.qrc
    NO_RESOURCE_TARGET_PATH
)

if(ANDROID)
    message(STATUS "*************** ANDROID SETTINGS ******************")
    set_target_properties(Gobblet PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android"
        QT_ANDROID_ABIS "arm64-v8a;x86_64"
        QT_ANDROID_MIN_SDK_VERSION 28
        QT_ANDROID_TARGET_SDK_VERSION 35
    )
    #qt_android_generate_deployment_settings(Gobblet)
elseif(APPLE)
    set_target_properties(Gobblet PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"
    )
else()
    install(TARGETS Gobblet
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

#qt_finalize_executable(Gobblet)
