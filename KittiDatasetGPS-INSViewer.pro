TEMPLATE = app

QT += location
QT += gui

CONFIG += c++1z

SOURCES += main.cpp \
    KittiDatasetTools/KittiDatasetParser.cpp \
    MapUpdater/MapUpdater.cpp

RESOURCES += qml.qrc

target.path = /app/KittiDatasetGPS-INSViewer/bin
INSTALLS += target

INCLUDEPATH += /app/EKF/include
DEPENDPATH += /app/EKF/include

INCLUDEPATH += /app/EKF/3rdparty
DEPENDPATH += /app/EKF/3rdparty

INCLUDEPATH += /usr/include/eigen3
DEPENDPATH += /usr/include/eigen3

HEADERS += \
    KittiDatasetTools/KittiDatasetParser.h \
    MapUpdater/MapUpdater.h

unix:!macx: LIBS += -L$$PWD/lib/ -lEKF-INS

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: PRE_TARGETDEPS += $$PWD/lib/libEKF-INS.a
