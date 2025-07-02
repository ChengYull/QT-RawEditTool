QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    paintwidget.cpp \
    rawedittool.cpp \
    widget.cpp

HEADERS += \
    paintwidget.h \
    rawedittool.h \
    widget.h

FORMS += \
    paintwidget.ui \
    widget.ui

INCLUDEPATH+= "D:/ProgramPackege/C++/Opencv/Opencv_mingw3.4.8/include/"
              # D:/ProgramPackege/C++/Opencv/Opencv_mingw3.4.8/include/opencv/
              # D:/ProgramPackege/C++/Opencv/Opencv_mingw3.4.8/include/opencv2/

LIBS += -L"D:/ProgramPackege/C++/Opencv/Opencv_mingw3.4.8/x64/mingw/bin"
LIBS += -lopencv_core348 \
        -lopencv_highgui348 \
        -lopencv_imgproc348 \
        -lopencv_imgcodecs348

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    _Idx1__Raw_20250402T172520.4064X3048.raw8_grbg.vcmpos_843.raw
