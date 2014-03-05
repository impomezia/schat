#!/bin/bash
set -e

echo "#define SCHAT_REVISION $SCHAT_REVISION" >  ../../src/common/revision.h
echo "#define SCHAT_PLATFORM \"ubuntu\""      >> ../../src/common/revision.h
echo "GIT_TIMESTAMP = `git log -n 1 --format=format:"%at"`" >> ../../src/common/config.pri
echo "GIT_REVISION = `git rev-parse HEAD`" >> ../../src/common/config.pri
