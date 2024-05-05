#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
NORMAL="\033[0m"
SERVER="localhost"
PORT="8080"
ORIGIN="http://${SERVER}:${PORT}"

# GET tests
get_test=(
    "curl -X GET ${ORIGIN}/ -H 'Host:localhost:8080' 400"
)

# POST tests
post_test=(
    "curl -X POST ${ORIGIN}/upload/test 400"
)

telnet_test=(
    "telnet ${SERVER} ${PORT} <<EOF
    GET / HTTP/1.1
    Host: localhost:8080

    GET / HTTP/1.1
    Host: localhost:8080

    GET / HTTP/1.1
    Host: localhost:8080

    GET / HTTP/1.1
    Host: localhost:8080

    GET / HTTP/1.1
    Host: localhost:8080

    GET / HTTP/1.1
    Host: localhost:8080

    EOF"
)

function run_and_check_curl_command() {
    local cmd="$1"
    local expected_code=${cmd##* }

    # Execute curl command and capture HTTP status code
    http_code=$(eval "${cmd% *} -s -o /dev/null -w \"%{http_code}\"")
    if [[ $http_code -eq $expected_code ]]; then
        echo -e "${GREEN}OK (${http_code})${NORMAL} -> $cmd"
    else
        echo -e "${RED}KO (${http_code})${NORMAL} -> $cmd"
    fi
}

echo "Running GET requests..."
for cmd in "${get_test[@]}"; do
    run_and_check_curl_command "$cmd"
done
echo

echo "Running POST requests..."
for cmd in "${post_test[@]}"; do
    run_and_check_curl_command "$cmd"
done
echo
