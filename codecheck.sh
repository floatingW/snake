#!/bin/bash

cpp_check_output=cpp_check_warning_output
rm $cpp_check_output

cppcheck \
--project=build/compile_commands.json \
--enable=warning,style,unusedFunction \
--std=c++20 \
-j 8 \
--output-file=$cpp_check_output \
--xml

clang_check_output=clang_check_warning_output
rm $clang_check_output

run-clang-tidy \
-quiet \
-j 8 \
-p build \
-export-fixes $clang_check_output \
-checks=-\*,bugprone-\*,clang-analyzer-\*,cppcoreguidelines-\*,hicpp-\*,modernize-\*,performance-\*,portability-\*,readability-\*