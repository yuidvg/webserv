#!/bin/bash
SERVER_ADDRESS="${1:-localhost:8080}"

siege -c 100 -r 100 http://${SERVER_ADDRESS} -b
