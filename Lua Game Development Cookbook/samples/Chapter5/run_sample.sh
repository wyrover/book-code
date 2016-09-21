#!/bin/sh

PROCESSOR_ARCHITECTURE=`uname -m`

if [ "${PROCESSOR_ARCHITECTURE}" = "x86" ]; then
	ARCHITECTURE="x86"
else
	if [ "${PROCESSOR_ARCHITECTURE}" = "x86_64" ]; then
		ARCHITECTURE="x86_64"
	fi
fi

export ROOT_PREFIX="../../common"
export BIN_PATH="${ROOT_PREFIX}/lib/external/platform-specific/Linux/${ARCHITECTURE}"
export LUA_INTERPRETER="${BIN_PATH}/lua5.1"
export LUA_INIT="__ROOT_PREFIX = os.getenv('ROOT_PREFIX') or '../../common';local core = dofile(__ROOT_PREFIX..'/lib/core.lua')"
export PATH=".:${BIN_PATH}:${PATH}"
export LD_LIBRARY_PATH="${BIN_PATH}:${LD_LIBRARY_PATH}"
${LUA_INTERPRETER} $*
