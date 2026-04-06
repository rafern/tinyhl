#!/bin/sh
set -e
./clean.sh
tar -czvf project.tar.gz ./*
export.lua project.tar.gz
rm project.tar.gz
