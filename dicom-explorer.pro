#-------------------------------------------------
#
# Project created by QtCreator 2018-02-10T16:51:08
#
#-------------------------------------------------

QT       += core gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dicom-explorer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES -= UNICODE  #DCMTK不支持Unicode,此处如果使用Unicode会编译错误。

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
        main.cpp \
        mainwindow.cpp \
    creaddcmfile.cpp

HEADERS += \
        mainwindow.h \
    creaddcmfile.h

FORMS += \
        mainwindow.ui

win32:{
    contains(QT_ARCH, i386){
        My_Target_Arch = 32-bit
    }
    else{
        My_Target_Arch = 64-bit
    }

    CONFIG(release, debug|release){
        My_Config_def = release
    }
    else{
        My_Config_def = debug
    }
}


# add by g.x.qiao
win32: INCLUDEPATH += $$PWD/dcmtk/win/include//$$My_Target_Arch
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/config
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/config

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/ofstd
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/ofstd

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/ofstd/diag
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/ofstd/diag

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/ofstd/variadic
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/ofstd/variadic

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/oflog
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/oflog/config
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/config

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/oflog/helpers
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/helpers

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/oflog/internal
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/internal

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/oflog/thread
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/thread

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/oflog/spi
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/spi

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/dcmjpeg
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/dcmjpeg

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/$$My_Target_Arch/dcmtk/dcmdata
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/dcmdata

unix: INCLUDEPATH += /usr/include/c++/5

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -lcharset_o
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -lcharset_d
else:unix: LIBS += -lcharset

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -llibiconv_o
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -llibiconv_d
else:unix: LIBS += -liconv

win32: LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -lofstd
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -lofstd

win32: LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -loflog
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -loflog

win32: LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -ldcmjpeg
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -ldcmjpeg

win32: LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -ldcmimage
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -ldcmimage

win32: LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -ldcmimgle
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -ldcmimgle

win32: LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -lijg8
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -lijg8

win32: LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -lijg12
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -lijg12

win32: LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -lijg16
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -lijg16

win32: LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -ldcmdata
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -ldcmdata

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -lzlib_o
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/$$My_Target_Arch/$$My_Config_def/ -lzlib_d
unix:!macx: LIBS += -lz

win32: LIBS += -lNetAPI32
win32: LIBS += -lWSock32
win32: LIBS += -lWS2_32
win32: LIBS += -lIPHlpApi
win32: LIBS += -lAdvAPI32

# g.x.qiao add end

RESOURCES += \
    icon.qrc

RC_ICONS = res/dicom-explorer-32.ico
