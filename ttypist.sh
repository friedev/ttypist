#!/bin/sh
set -eu
export PATH='/bin:/sbin:/usr/bin:/usr/local/bin'

usage="Usage: $0 word_list word_count"
word_list=${1:?$usage}
word_count=${2:?$usage}

# Assume the ttypist executable is located in the same directory as this script
src_dir=$(realpath "$(dirname "$0")")
ttypist=$src_dir/ttypist

# Generate a test string:
# 1. Shuffle the word list and output the desired number of words
# 2. Concatenate them into a single string using echo
test_str=$(echo $(shuf "$word_list" --head-count="$word_count"))

# Run ttypist and propagate its return code
$ttypist "$test_str"
exit $?
