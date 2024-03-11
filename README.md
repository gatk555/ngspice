This repository contains a copy of the main development branch of Ngspice (https://ngspice.sourceforge.io/) and scripts for Github Actions that run daily to update the copy and build it for Linux (X64) and MacOS (X86_64 and arm64).  The resulting binaries can be found under the "Actions" tab above.  (Look for "artifacts".)  A Github login is required for downloading.

The binaries are packaged as ZIP files so the execute permission is not preserved.  Use this command to restore it:

	chmod a+x bin/ngspice

and for a Macintosh also:

	xattr -d com.apple.quarantine bin/ngspice
	xattr -d com.apple.quarantine lib/ngspice/*

The packaged files are intended for installation in /usr/local.
A file, /usr/local/README.ngspice, identifies the latest revision that
was included in the build.

This branch is for this file and Github Actions workflow files only, the ngspice source is on a branch with a name of the form pm_XX, depending on the current Sourceforge development branch.  There is currently one local change to the source: the MacOS compile script has been modified to support cross-compiling for arm64.
