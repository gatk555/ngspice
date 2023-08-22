This repository contains a copy of the main development branch (pre-master-42)
of Ngspice (https://ngspice.sourceforge.io/) and scripts for Github Actions
that run daily to update the copy and build it.  The resulting binaries
can be found under the "Actions" tab above.  (Look for "artifacts".)
A Github login is required for downloading.

The binaries are packaged as ZIP files so the execute permission
is not preserved.  Use this command to restore it:

	chmod a+x bin/ngspice

and for a Macintosh also:

	xattr -d com.apple.quarantine bin/ngspice
	xattr -d com.apple.quarantine lib/ngspice/*

The packaged files are targeted at /usr/local but may be installed anywhere
and ngspice will work correctly if environment variable SPICE_ROOT
is set to that location.

This branch is for Github Actions workflow files only, the ngspice source
is on branch gh_pm42.  There are currently two local changes to the source:
code has been added to support SPICE_ROOT and the MacOS compile script
has been modified to support cross-compiling for arm64.
