#!/bin/bash

cmake -G "Ninja" -Wno-dev -S . -B ./Build -DCMAKE_BUILD_TYPE=$2
cd ./Build
make
./$1