#!/bin/bash

cmake -G "Unix Makefiles" -S . -B ./Build -DCMAKE_BUILD_TYPE=$2
cd ./Build
make
./$1