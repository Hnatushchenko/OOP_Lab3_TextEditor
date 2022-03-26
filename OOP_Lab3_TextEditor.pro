QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

VERSION = 0.0.0.1

QMAKE_TARGET_COMPANY = national university lviv polytechnic
QMAKE_TARGET_PRODUCT = Simple text editor
QMAKE_TARGET_DESCRIPTION = The program allows a user to enter, change, store, and print text
QMAKE_TARGET_COPYRIGHT = Andrii Hnatushchenko

SOURCES += \
    main.cpp \
    texteditor.cpp

HEADERS += \
    texteditor.h

FORMS += \
    texteditor.ui

RC_ICONS = notepad.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
