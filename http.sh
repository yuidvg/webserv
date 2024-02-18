#!/bin/bash

# curlコマンドのリストを配列として定義
commands=(
    "curl -X GET http://localhost:8080/ -v"
    "curl -X POST -d \"nickname=test\" http://localhost:8080/upload -v"
    "curl -X DELETE http://localhost:8080/uploadPath/upload.txt -v"
)

# コマンドを一つずつ実行
for cmd in "${commands[@]}"; do
    echo -e "\033[32m${cmd}\033[0m"
    eval ${cmd}
    echo
done
