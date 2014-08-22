TEMPLATE       = lib
CONFIG         += plugin static
lessThan(QT_MAJOR_VERSION, 5):!win32-msvc*:QMAKE_CXXFLAGS += -std=c++0x

DEPENDPATH += \
    . \
    ../../../ \
    ../../../../../../ \

VPATH += \
    . \
    ../../../ \
    ../../../../../../ \

INCLUDEPATH += \
    . \
    ../../../ \
    ../../../../../../ \

CONFIG(debug, debug|release) { 
  RCC_DIR = ../../../../tmp/$${TEMPLATE}/$${TARGET}/debug/rcc
  MOC_DIR = ../../../../tmp/$${TEMPLATE}/$${TARGET}/debug/moc
  OBJECTS_DIR = ../../../../tmp/$${TEMPLATE}/$${TARGET}/debug/obj
  DESTDIR = ../../../../../../../../out/debug
} else {
  RCC_DIR = ../../../../tmp/$${TEMPLATE}/$${TARGET}/release/rcc
  MOC_DIR = ../../../../tmp/$${TEMPLATE}/$${TARGET}/release/moc
  OBJECTS_DIR = ../../../../tmp/$${TEMPLATE}/$${TARGET}/release/obj
  DESTDIR = ../../../../../../../../out
}
