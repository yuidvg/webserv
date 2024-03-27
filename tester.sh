#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
NORMAL="\033[0m"

# コマンドと期待されるステータスコードのペア
curl_commands=(
    "curl -X GET http://localhost:8080/ 200"
    "curl -X POST -d \"nickname=test\" http://localhost:8080/upload 200"
    "curl -X DELETE http://localhost:8080/uploadPath/upload.txt 200"
    "curl -X GET http://localhost:8080/autoindex 400"
    "curl -X GET http://localhost:8080/cgi-bin/helloWorld.cgi 200"
    "curl -X GET http://localhost:8080/cgi-bin/redirAfterFiveMinutes.cgi 200"
    "curl -X GET http://localhost:8080/cgi-bin/submit.cgi 200"
    "curl -X GET http://localhost:8080/nothing 400"
    "curl -X DELETE http://localhost:8080/uploadPath/nothing 400"
    "curl -X POST -d \"nickname=test\" http://localhost:8080/nothing 400"
    "curl -X GET http://localhost:8080/cgi-bin/redirect.cgi 302"
    "curl -X GET http://localhost:8080/cgi-bin/localRedirect.cgi 302"
)

function run_and_check_curl_command() {
    local cmd="$1"
    local expected_code=${cmd##* }

    # -s でサイレントモード、-w でHTTPステータスコード出力
    http_code=$(eval ${cmd% *} -s -o response.txt -w "%{http_code}")
    if [[ $http_code -eq $expected_code ]]; then
        echo -en "${GREEN}OK (${http_code})"
    else
        echo -en "${RED}KO (${http_code})"
    fi
    echo -ne "${NORMAL}"
    echo " -> ${cmd}"
}

echo "Running curl commands..."
for cmd in "${curl_commands[@]}"; do
    run_and_check_curl_command "$cmd"
done
