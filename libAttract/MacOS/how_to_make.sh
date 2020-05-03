#!/bin/bash
SCRIPTPATH=$(greadlink -f "$BASH_SOURCE")
APPDIR=$(dirname "$SCRIPTPATH")
printf "Universal Makefile allows building both static/shared libraries and examples;\n"
printf "make without parameters builds release target.\n\n"
printf "NB! DO NOT select both debug&release in one run they're exclusive\n"
printf "To build lib add \"lib\" specifier\n"
printf "	Ex: \$make lib release\n"
printf "To build only examples and link with lib binary call make <target>\n"
printf "	Ex: \$make debug\n"
printf "mostlyclean removes d and .o files from both targets\n"
printf "	Ex: \$make clean\n"
printf "clean additionally removes removes examples binaries\n"
printf "	Ex: \$make clean\n"
printf "clobber additionally removes library binaries too\n"
printf "	Ex: \$make clobber\n"
printf "\n Will now build examples\n"
make release --silent
make debug --silent
make mostlyclean --silent
