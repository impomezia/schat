TARGET = RupProviderPlugin
QT = core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS = \
   RupProviderPlugin.h \
   RupSettings.h \
   RupUploader.h \

SOURCES = \
   RupProviderPlugin.cpp \
   RupSettings.cpp \
   RupUploader.cpp \
 
include(../../plugins.pri)
