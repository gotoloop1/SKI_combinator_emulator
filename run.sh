#!/usr/bin/env bash
make

echo "Please input formula of SKI combinator"

exec ./turing_machine SKI_combinator_emulator.txt
