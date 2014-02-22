#!/bin/bash
set -e

echo "#define SCHAT_REVISION $SCHAT_REVISION" >  ../../src/common/revision.h
echo "#define SCHAT_PLATFORM \"ubuntu\""      >> ../../src/common/revision.h