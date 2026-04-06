#!/bin/sh
set -e

echo "running out/flash"
LD_LIBRARY_PATH=./out ./out/flash
