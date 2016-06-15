#!/bin/bash
git ls-files '*.cc' '*.cpp' '*.h' | xargs clang-format -i
