#!/bin/bash
SCRIPTPATH=$(readlink -f "$BASH_SOURCE")
APPDIR=$(dirname "$SCRIPTPATH")
export LD_LIBRARY_PATH=$APPDIR:${LD_LIBRARY_PATH}
ldd ./example
./example
