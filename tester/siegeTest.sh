#!/bin/bash

SERVER="localhost"
PORT="8080"
ORIGIN="http://${SERVER}:${PORT}"

siege -c 10 -t10S -r 100 ${ORIGIN} -b
