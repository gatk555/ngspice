 ## Process this file with automake to produce Makefile.in

MAINTAINERCLEANFILES = Makefile.in

# Verilator support: files installed to script directory and below.

initdatadir = $(pkgdatadir)/scripts
initdata_DATA = vlnggen

initdata1dir = $(pkgdatadir)/scripts/src
initdata1_DATA = verilator_shim.cpp verilator_main.cpp

initdata2dir = $(pkgdatadir)/scripts/src/ngspice
initdata2_DATA = ../../include/ngspice/cosim.h \
		 ../../include/ngspice/miftypes.h \
                 ../../include/ngspice/cmtypes.h

# Icarus Verilog support: build two shared libraries.

pkglib_LTLIBRARIES = ivlng.la ivlngvpi.la
ivlng_la_SOURCES = icarus_shim.c icarus_shim.h
ivlng_la_CFLAGS = -I../../../../src/include
ivlng_la_LDFLAGS = -module -shared -avoid-version

ivlngvpi_la_SOURCES = vpi.c icarus_shim.h vpi_user.fake
ivlngvpi_la_CFLAGS = -I../../../../src/include
ivlngvpi_la_LDFLAGS = -module -shared  -avoid-version

if DLIBS_FULLY_RESOLVED
ivlng_la_LDFLAGS += -no-undefined
ivlngvpi_la_LDFLAGS += -no-undefined -lvvp -L.libs -Wl,-l:ivlng.dll
endif

# Libtool makes and installs static libraries, remove them after installation.

install-exec-hook:
	cd $(DESTDIR)$(pkglibdir); \
	rm -f ivlng*.la ; \
	mv ivlngvpi.* ivlng.vpi

uninstall-hook:
	rm -f $(DESTDIR)$(pkglibdir)/ivlng.vpi $(DESTDIR)$(pkglibdir)/ivlng.so
