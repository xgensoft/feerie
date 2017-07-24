#!/bin/bash
#make clean
rm -R CMakeFiles
rm CMakeCache.txt
rm Makefile
rm cmake_install.cmake
rm CMakeLists.txt.user
rm -R bin/client/*
rm -R bin/editor/*
rm -R bin/server/*
rm -R bin/tests/*
rm -R bin/lighttpd/*
rm -R lib/core
rm -R external/*
find . -not -path "*cmake*" -not -iname CMakeLists.txt -not -iname CMakeLists.txt.local -iname '*cmake*' -exec rm -rf {} \+
find . -type d -iname "Testing" -exec rm -rf {} \+
find . -iname 'Makefile' -exec rm -rf {} \+
find . -iname 'Doxyfile' -exec rm -rf {} \+