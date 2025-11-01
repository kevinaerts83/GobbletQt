# gobblet_ai/CMakeLists.txt
qt_add_library(gobblet_ai STATIC
    ai.cpp ai.h
    aimove.cpp aimove.h
)

target_include_directories(gobblet_ai PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 REQUIRED COMPONENTS Test Core)
add_executable(myTest test_ai.cpp)
target_link_libraries(myTest PRIVATE Qt6::Test gobblet_ai)

set_target_properties(gobblet_ai PROPERTIES EXCLUDE_FROM_ALL OFF)
