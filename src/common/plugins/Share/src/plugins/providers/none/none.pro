TARGET = NoneProviderPlugin
QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS = \
   NoneProviderPlugin.h \
   NoneUploader.h \

SOURCES = \
   NoneProviderPlugin.cpp \
   NoneUploader.cpp \
 
include(../../plugins.pri)
