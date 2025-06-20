#!/bin/sh
make profile
./main
gprof main gmon.out > profile.txt
