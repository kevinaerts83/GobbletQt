# blues/CMakeLists.txt
qt_add_library(blues STATIC
    chatclient.cpp chatclient.h
    chatserver.cpp chatserver.h
    bluetoothManager.cpp bluetoothManager.h
)

target_include_directories(blues PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link blues and Qt
target_link_libraries(blues
    PUBLIC
        Qt6::Core
        Qt6::Bluetooth
)

set_target_properties(blues PROPERTIES EXCLUDE_FROM_ALL OFF)
