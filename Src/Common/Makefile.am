noinst_LTLIBRARIES = libdashermisc.la
libdashermisc_la_SOURCES = \
                AppSettingsData.h \
                AppSettingsHeader.h \
		Common.h \
		Globber.h \
		Globber.cpp \
		Hash.h \
		I18n.h \
		MSVC_Unannoy.h \
		myassert.h \
		mydebug.h \
		ModuleSettings.h \
		NoClones.h \
		Random.h \
                Trace.cpp \
		Trace.h \
		Allocators/PooledAlloc.h \
		Allocators/SimplePooledAlloc.h \
		Platform/stdminmax.h \
		Types/int.h \
		Types/int32.h \
		Types/int64.h

#		IOstreamDasherEdit.cc   I have no idea what this does, but it's broken now
#		IOstreamDasherEdit.h 

libdashermisc_la_LIBADD = $(LTLIBOBJS)

AM_CXXFLAGS = -I$(srcdir)/../DasherCore

EXTRA_DIST = Common.vcproj Common_vc80.vcproj
