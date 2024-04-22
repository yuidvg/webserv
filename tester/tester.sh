#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
NORMAL="\033[0m"
SERVER_ADDRESS="${1:-localhost:8080}"

curl_commands=(
    "curl -X GET http://${SERVER_ADDRESS}/ 200"
    "curl -X POST -d \"nickname=test\" http://${SERVER_ADDRESS}/uploadPath 200"
    "curl -X DELETE http://${SERVER_ADDRESS}/uploadPath/uploadPath.txt 200"
    "curl -X GET http://${SERVER_ADDRESS}/autoindex 200"
    "curl -X GET http://${SERVER_ADDRESS}/nothing 400"
    "curl -X GET http://${SERVER_ADDRESS}/nothing 400"
    "curl -X POST -d \"nickname=test\" http://${SERVER_ADDRESS}/nothing 400"
    "curl -X DELETE http://${SERVER_ADDRESS}/uploadPath/nothing 400"
    "curl -X GET http://${SERVER_ADDRESS}/ -H 'Host: ${SERVER_ADDRESS}' 200"
    "curl -X GET http://${SERVER_ADDRESS}/ -H 'Host:${SERVER_ADDRESS}' 400"
    "curl -X POST http://${SERVER_ADDRESS}/uploadPath/test -H 'Host: ${SERVER_ADDRESS}' -H 'Content-Length: 11' -d \"Hello World\" 200"
    "curl -X POST http://${SERVER_ADDRESS}/uploadPath/test -H 'Host: ${SERVER_ADDRESS}' -H 'Content-Length: -9' -d \"123456789\" 400"
    "curl -X POST http://${SERVER_ADDRESS}/uploadPath/test -H 'Host: ${SERVER_ADDRESS}' -H 'Content-Length: 100' -d \"123456789\" 400"
    "curl -X GET http://${SERVER_ADDRESS}/cgi-bin/helloWorld.cgi 200"
    "curl -X GET http://${SERVER_ADDRESS}/cgi-bin/redirAfterFiveMinutes.cgi 200"
    "curl -X GET http://${SERVER_ADDRESS}/cgi-bin/submit.cgi 200"
    "curl -X GET http://${SERVER_ADDRESS}/cgi-bin/redirect.cgi 302"
    "curl -X GET http://${SERVER_ADDRESS}/cgi-bin/localRedirect.cgi 302"
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

# その他のコマンド
other_commands=(
    "ruby tester/chunkRequest.rb"
)
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

echo "Running other commands..."
for cmd in "${other_commands[@]}"; do
    run_other_command "$cmd"
done
# siege -c 10 -r 10 http://${SERVER_ADDRESS} -b
