lib_LTLIBRARIES = libcamsrcjpegenc.la

includelibcamsrcjpegencdir = $(includedir)
includelibcamsrcjpegenc_HEADERS = include/camsrcjpegenc_type.h \
                                  include/camsrcjpegenc_sub.h \
                                  include/camsrcjpegenc.h

libcamsrcjpegenc_la_SOURCES = camsrcjpegenc.c

libcamsrcjpegenc_la_CFLAGS = -I$(srcdir)/include \
                             $(GLIB_CFLAGS) \
                             $(GMODULE_CFLAGS)

libcamsrcjpegenc_la_LIBADD = $(GLIB_LIBS) \
                             $(GMODULE_LIBS)

if USE_DLOG
libcamsrcjpegenc_la_CFLAGS += $(DLOG_CFLAGS) -DUSE_DLOG

libcamsrcjpegenc_la_LIBADD += $(DLOG_LIBS)
endif

