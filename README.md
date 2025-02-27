This repository contains a copy of the main development branch of Ngspice (https://ngspice.sourceforge.io/) and scripts for Github Actions that run daily to update the copy and build it. There are builds for Linux (X64, gcc compiler), Windows (X64, both MSVC and MSYS) and MacOS (clang).  The resulting binaries can be found under the "Actions" tab above.  (Look for "artifacts".)  A Github login is required for downloading.

The binaries are packaged as ZIP files intended to be expanded to /usr/local or C:\Spice64.  For Unix-like OSs the execute permission is not preserved.  Use this command to restore it:

	chmod a+x bin/ngspice

and for a Macintosh this may also be needed:

	xattr -d com.apple.quarantine bin/ngspice
	xattr -d com.apple.quarantine lib/ngspice/*

A file, /usr/local/README.ngspice, identifies the origin of the files, and the latest revision that was included in the build.

This branch is for this file and Github Actions workflow files only, the ngspice source is on a branch with a name of the form pm_XX, depending on the current Sourceforge development branch.
