#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
NORMAL="\033[0m"
SERVER="localhost"
PORT="8080"
ORIGIN="http://${SERVER}:${PORT}"

cgi_test=(
    "curl ${ORIGIN}/cgi-bin/documentResponse.cgi 200"
    "curl -X POST ${ORIGIN}/cgi-bin/documentResponse.cgi -d \"nickname=test\" 200"
    # "curl ${ORIGIN}/cgi-bin/localRedirectResponse.cgi -L 302"
    # "curl -X POST ${ORIGIN}/cgi-bin/localRedirectResponse.cgi -L -d \"nickname=test\" 302"
    # "curl ${ORIGIN}/cgi-bin/clientRedirectResponse.cgi -L 302"
    # "curl -X POST ${ORIGIN}/cgi-bin/clientRedirectResponse.cgi -L -d \"nickname=test\" 302"
    "curl ${ORIGIN}/cgi-bin/clientRedirectResponseWithDocument.cgi 200"
    "curl -X POST ${ORIGIN}/cgi-bin/clientRedirectResponseWithDocument.cgi -d \"nickname=test\" 200"
    "curl ${ORIGIN}/cgi-bin/submit.cgi 200"
    "curl -X POST ${ORIGIN}/cgi-bin/submit.cgi -d \"nickname=test\" 200"
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

echo "Running CGI requests..."
for cmd in "${cgi_test[@]}"; do
	run_and_check_curl_command "$cmd"
done
