TARGET = GeekpicProviderPlugin
QT = core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS = \
   GeekpicProviderPlugin.h \
   GeekpicUploader.h \

SOURCES = \
   GeekpicProviderPlugin.cpp \
   GeekpicUploader.cpp \
 
include(../../plugins.pri)
