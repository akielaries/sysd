#!/bin/sh
find . -regex '.*\.\(c\|cu\|cuh\|cpp\|hpp\|h\|cxx\)' -exec clang-format-15 -style=file -i {} \;
