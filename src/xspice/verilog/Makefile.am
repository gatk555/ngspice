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
# The "x_" prefixes help to remove unwanted files after installation.

pkglib_LTLIBRARIES = x_ivlng.la x_ivlngvpi.la
x_ivlng_la_SOURCES = icarus_shim.c icarus_shim.h
x_ivlng_la_CFLAGS = -I../../../../src/include
x_ivlng_la_LDFLAGS = -module -shared -version-number 0

x_ivlngvpi_la_SOURCES = vpi.c icarus_shim.h vpi_user.fake
x_ivlngvpi_la_CFLAGS = -I../../../../src/include
x_ivlngvpi_la_LDFLAGS = -module -shared  -version-number 0

if DLIBS_FULLY_RESOLVED
x_ivlng_la_LDFLAGS += -no-undefined
x_ivlngvpi_la_LDFLAGS += -no-undefined -lvvp x_ivlngvpi.dll.a
endif

# Libtool makes and installs static libraries and symbolic links.
# Rename and tidy after installation.

install-exec-hook:
	cd $(DESTDIR)$(pkglibdir); \
	cp x_ivlng.so ivlng.so; \
	cp x_ivlngvpi.so ivlng.vpi; \
	rm x_ivlng*

uninstall-hook:
	rm -f $(DESTDIR)$(pkglibdir)/ivlng.vpi $(DESTDIR)$(pkglibdir)/ivlng.so
