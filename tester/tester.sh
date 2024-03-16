#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
NORMAL="\033[0m"

curl_commands=(
    "curl -X GET http://localhost:8080/"
    "curl -X POST -d \"nickname=test\" http://localhost:8080/upload"
    "curl -X DELETE http://localhost:8080/uploadPath/upload.txt"
    "curl -X GET http://localhost:8080/autoindex"
    "curl -X GET http://localhost:8080/cgi-bin/helloWorld.cgi"
    "curl -X GET http://localhost:8080/cgi-bin/redirAfterFiveMinutes.cgi"
    "curl -X GET http://localhost:8080/cgi-bin/submit.cgi"
    "curl -X GET http://localhost:8080/cgi-bin/redirect.cgi"
    "curl -X GET http://localhost:8080/cgi-bin/localRedirect.cgi"
)

other_commands=(
    "ruby tester/chunkRequest.rb"
)

function run_and_check_curl_command() {
    local cmd=$1

    # -s でサイレントモード、-w でHTTPステータスコード出力
    http_code=$(eval ${cmd} -s -o response.txt -w "%{http_code}")
    if [[ $http_code -ge 200 && $http_code -le 299 ]]; then
        echo -en "${GREEN}OK (${http_code})"
    elif [[ $http_code -ge 300 && $http_code -le 399 ]]; then
        echo -en "${GREEN}OK (${http_code})"
    elif [[ $http_code -ge 400 && $http_code -le 499 ]]; then
        echo -en "${RED}Client Error (${http_code})"
    elif [[ $http_code -ge 500 && $http_code -le 599 ]]; then
        echo -en "${RED}Server Error (${http_code})"
    else
        echo -en "${RED}Unknown code: ${http_code}"
    fi
    echo -ne "${NORMAL}"
    echo " -> ${cmd}"
}

function run_other_command() {
    local cmd=$1

    echo "Running: ${cmd}"
    eval ${cmd}
    if [[ $? -eq 0 ]]; then
        echo -en "${GREEN}OK"
    else
        echo -en "${RED}KO"
    fi
    echo -e "${NORMAL} -> ${cmd}"
}

echo "Running curl commands..."
for cmd in "${curl_commands[@]}"; do
    run_and_check_curl_command "$cmd"
done

echo "Running other commands..."
for cmd in "${other_commands[@]}"; do
    run_other_command "$cmd"
done
