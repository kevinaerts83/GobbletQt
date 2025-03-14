TEMPLATE = app

QT += quick

CONFIG += qt quick c++11
CONFIG += android
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
ANDROID_ABIS = arm64-v8a
ANDROID_MIN_SDK_VERSION = 21
ANDROID_TARGET_SDK_VERSION = 34
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ai.cpp \
        aimove.cpp \
        board.cpp \
        board3d.cpp \
        gobbler.cpp \
        gobbler3d.cpp \
        grades.cpp \
        main.cpp \
        matrix.cpp \
        mediator.cpp \
        shape.cpp \
        shape3d.cpp \
        tilecounter.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ai.h \
    aimove.h \
    board.h \
    board3d.h \
    bridge.h \
    gobbler.h \
    gobbler3d.h \
    grades.h \
    matrix.h \
    mediator.h \
    setupBoard.h \
    setupMenu.h \
    shape.h \
    shape3d.h \
    tilecounter.h

CONFIG += qmltypes
QML_IMPORT_NAME = Gobbler
QML_IMPORT_MAJOR_VERSION = 1

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/res/xml/qtprovider_paths.xml
