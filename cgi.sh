#!/bin/bash

実行するURLのリストを配列として定義
commands=(
    "curl -X GET http://localhost:8080/cgi-bin/helloWorld.cgi -v"
    "curl -X GET http://localhost:8080/cgi-bin/redirAfterFiveMinutes.cgi -v"
    "curl -X GET http://localhost:8080/cgi-bin/submit.cgi -v"
    "curl -X GET http://localhost:8080/cgi-bin/redirect.cgi -v"
    "curl -X GET http://localhost:8080/cgi-bin/localRedirect.cgi -v"
)

# コマンドを一つずつ実行
for cmd in "${commands[@]}"; do
    echo -e "\033[32m${cmd}\033[0m"
    eval ${cmd}
    echo
done

# urls=(
#     "http://localhost:8080/cgi-bin/helloWorld.cgi"
#     "http://localhost:8080/cgi-bin/redirAfterFiveMinutes.cgi"
#     "http://localhost:8080/cgi-bin/submit.cgi"
#     "http://localhost:8080/cgi-bin/redirect.cgi"
#     "http://localhost:8080/cgi-bin/localRedirect.cgi"
# )

# # 各URLに対してGET, POST, DELETEメソッドを実行
# for url in "${urls[@]}"; do
#     for method in GET DELETE; do
#         echo -e "\033[32mcurl -X $method $url -v\033[0m"
#         curl -X $method $url -v
#         echo
#         sleep 1 # サーバーに負荷をかけないように間隔を設ける
#     done
# done
