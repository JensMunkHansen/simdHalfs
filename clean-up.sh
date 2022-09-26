#!/bin/bash
rm -Rf Makefile.in ./src/Makefile.in aclocal.m4 ./autom4te.cache
rm -f configure
rm -f Makefile
rm -f ./src/Makefile
rm -f config.log config.status
rm -f ./Debug/*
rm -f ./Release/*
rm -f ./Debug-pthreads/*
rm -f ./x64/Debug/*
rm -f ./x64/Release/*
rm -f *.ncb
rm -f ./include/config.h.in
rm -f ./include/config.h
rm -f ./include/stamp-h1
find . -name \*~ | xargs -i -t rm -f {}
rm -f *.suo
