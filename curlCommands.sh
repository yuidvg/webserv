#!/bin/bash

# curlコマンドのリストを配列として定義
commands=(
    "curl -X GET http://localhost:8080/"
    "curl -X POST -d \"nickname=test\" http://localhost:8080/upload"
    "curl -X DELETE http://localhost:8080/uploadPath/upload.txt"
    "curl http://localhost:8080/cgi-bin.cgi"
    "curl http://localhost:8080/RedirAfterFiveMinutes.cgi"
    "curl http://localhost:8080/submit.cgi"
)

# コマンドを一つずつ実行
for cmd in "${commands[@]}"; do
    echo -e "\033[32m${cmd}\033[0m"
    eval ${cmd}
    echo
    sleep 2
done
