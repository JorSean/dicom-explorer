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


# add by g.x.qiao
win32: INCLUDEPATH += $$PWD/dcmtk/win/include/
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/config
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/config

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/ofstd
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/ofstd

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/ofstd/diag
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/ofstd/diag

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/ofstd/variadic
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/ofstd/variadic

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/oflog
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/oflog/config
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/config

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/oflog/helpers
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/helpers

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/oflog/internal
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/internal

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/oflog/thread
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/thread

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/oflog/spi
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/oflog/spi

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/dcmjpeg
else:unix:  INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/dcmjpeg

win32: INCLUDEPATH += $$PWD/dcmtk/win/include/dcmtk/dcmdata
else:unix: INCLUDEPATH += $$PWD/dcmtk/linux/include/dcmtk/dcmdata

INCLUDEPATH += 'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Include'
INCLUDEPATH += /usr/include/c++/5

# g.x.qiao add end

DEPENDPATH += C:\Program Files\Microsoft SDKs\Windows\v6.0A\Lib\x64;

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -lcharset_d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -lcharset_d
else:unix: LIBS += -lcharset

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -llibiconv_d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -llibiconv_d
else:unix: LIBS += -liconv

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -lofstd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -lofstd
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -lofstd

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -loflog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -loflog
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -loflog

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -ldcmjpeg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -ldcmjpeg
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -ldcmjpeg

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -ldcmimage
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -ldcmimage
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -ldcmimage

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -ldcmimgle
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -ldcmimgle
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -ldcmimgle

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -lijg8
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -lijg8
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -lijg8

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -lijg12
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -lijg12
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -lijg12

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -lijg16
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -lijg16
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -lijg16

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -ldcmdata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -ldcmdata
else:unix: LIBS += -L$$PWD/dcmtk/linux/lib/ -ldcmdata

win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/x64/' -lNetAPI32
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/x64/' -lNetAPI32

win32: LIBS += -L'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/x64/' -lWSock32

win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/x64/' -lWS2_32
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/x64/' -lWS2_32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/release/ -lzlib_d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/dcmtk/win/lib/debug/ -lzlib_d
unix:!macx: LIBS += -lz

win32: LIBS += -L'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/' -lIPHlpApi

win32: LIBS += -lAdvAPI32
