#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
NORMAL="\033[0m"
SERVER="localhost"
PORT="8080"
ORIGIN="http://${SERVER}:${PORT}"

# GET tests
get_test=(
    "curl -X GET ${ORIGIN}/ 200"
    "curl -X GET ${ORIGIN}/nothing 400"
    "curl -X GET ${ORIGIN}/autoindex 200"
)

# POST tests
post_test=(
    "curl -X POST -d \"nickname=test\" ${ORIGIN}/upload/test 200"
    "curl -X POST -d \"nickname=test\" ${ORIGIN}/nothing/test 400"
    "curl -X POST ${ORIGIN}/upload/test1 -H 'Host: ${SERVER}' -H 'Content-Length: 11' -d \"Hello World\" 200"
    "curl -X POST ${ORIGIN}/upload/test2 -H 'Host: ${SERVER}' -H 'Content-Length: -9' -d \"123456789\" 400"
    "curl -X POST ${ORIGIN}/upload/test3 -H 'Host: ${SERVER}' -H 'Content-Length: 100' -d \"123456789\" 400"
)

# DELETE tests
delete_test=(
    "curl -X DELETE ${ORIGIN}/upload/test 200"
    "curl -X DELETE ${ORIGIN}/upload/nothing 400"
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

echo "\nRunning DELETE requests..."
for cmd in "${delete_test[@]}"; do
    run_and_check_curl_command "$cmd"
done
