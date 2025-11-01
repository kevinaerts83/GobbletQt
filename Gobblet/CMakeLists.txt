# Gobblet/CMakeLists.txt
qt_add_executable(Gobblet WIN32 MACOSX_BUNDLE
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

# Link everything together
target_link_libraries(Gobblet
    PRIVATE
        blues
        gobblet_ai
        Qt6::Core
        Qt6::Quick
        Qt6::Bluetooth
)

target_compile_definitions(Gobblet PRIVATE QT_DEPRECATED_WARNINGS)
