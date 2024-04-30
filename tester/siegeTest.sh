#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
NORMAL="\033[0m"
SERVER="localhost"
PORT="8080"
ORIGIN="http://${SERVER}:${PORT}"

siege -c 100 -t10S -r 100 ${ORIGIN} -b
