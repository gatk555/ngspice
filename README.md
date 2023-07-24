This repository contains a copy of the main development branch (pre-master)
of Ngspice (https://ngspice.sourceforge.io/) and scripts for Github Actions
that run daily to update the copy and build it.  The resulting binaries
can be found under the "Actions" tab above.  (Look for "artifacts".
A Github login is required for downloading.)

This branch is for Github Actions workflow files only, the ngspice source
is on branch gh_pm.  There are currently two local changes to the source:
the built tree (targeted at /usr/local) may be installed anywhere and
will work if environment variable SPICE_ROOT is set to that location;
and the MacOS compile script has been modified to support cross-compiling
for arm64.
