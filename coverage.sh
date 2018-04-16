#!/bin/sh

kcov --exclude-path=/usr/include,build/googletest-src coverage build/ate-chips roms/Fishie.ch8
kcov --exclude-path=/usr/include,build/googletest-src coverage build/ate-chips_tests
firefox coverage/index.html
