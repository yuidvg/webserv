#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
NORMAL="\033[0m"

commands=(
    "curl -X GET http://localhost:8080/"
    "curl -X POST -d \"nickname=test\" http://localhost:8080/upload"
    "curl -X DELETE http://localhost:8080/uploadPath/upload.txt"
)

function run_and_check_command() {
    local cmd=$1

    # -s でサイレントモード、-w でHTTPステータスコード出力
    http_code=$(eval ${cmd} -s -o response.txt -w "%{http_code}")
    if [[ $http_code -ge 200 && $http_code -le 299 ]]; then
        echo -en "${GREEN}OK"
    elif [[ $http_code -ge 400 && $http_code -le 499 ]]; then
        echo -en "${RED}KO (Client Error)"
    elif [[ $http_code -ge 500 && $http_code -le 599 ]]; then
        echo -en "${RED}KO (Server Error)"
    else
        echo -en "${RED}Unknown code: $code"
    fi
    echo -ne "${NORMAL}"
    echo " -> ${cmd}"
}

for cmd in "${commands[@]}"; do
    run_and_check_command "$cmd"
done
