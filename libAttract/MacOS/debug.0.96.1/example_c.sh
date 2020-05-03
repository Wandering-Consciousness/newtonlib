#!/bin/bash
SCRIPTPATH=$(greadlink -f "$BASH_SOURCE")
APPDIR=$(dirname "$SCRIPTPATH")
export DYLD_LIBRARY_PATH=$APPDIR:${DYLD_LIBRARY_PATH}
otool -L ./example_c
./example_c
