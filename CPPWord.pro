QT += core gui widgets printsupport

CONFIG += c++17

TARGET = CPPWord
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/texteditor.cpp \
    src/formatbar.cpp \
    src/documentmanager.cpp

HEADERS += \
    src/mainwindow.h \
    src/texteditor.h \
    src/formatbar.h \
    src/documentmanager.h

RESOURCES += \
    icons.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 