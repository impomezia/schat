HEADERS += \
   GeoIP/GeoIP.h \
   GeoIP/GeoIP_internal.h \
   GeoIP/GeoIPCity.h \
   GeoIP/global.h \
   GeoIP/types.h \

SOURCES += \
   GeoIP/GeoIP.c \
   GeoIP/GeoIPCity.c \
   GeoIP/regionName.c \
   GeoIP/timeZone.c \

win32 {
  DEFINES += _CRT_SECURE_NO_WARNINGS
  LIBS += -lwsock32 -lws2_32
}
