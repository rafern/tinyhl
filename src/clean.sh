#!/bin/sh
set -e
rm -f project.tar.gz out/flash out/lshldevs out/lshlredstone out/libtinyhl.so out/libtinyhl_redstone.so
if [ -d "out" ]; then rmdir out; fi
