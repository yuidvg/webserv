#!/bin/bash

SERVER="localhost"
PORT="8080"
ORIGIN="http://${SERVER}:${PORT}"

siege -c 100 -t2S -r 100 ${ORIGIN} -b
