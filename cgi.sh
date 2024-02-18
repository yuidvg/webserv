#!/bin/bash

# 実行するURLのリストを配列として定義
urls=(
    "http://localhost:8080/cgi-bin.cgi"
    "http://localhost:8080/RedirAfterFiveMinutes.cgi"
    "http://localhost:8080/submit.cgi"
    "http://localhost:8080/redirect.cgi"
    "http://localhost:8080/localRedirect.cgi"
)

# 各URLに対してGET, POST, DELETEメソッドを実行
for url in "${urls[@]}"; do
    for method in GET POST DELETE; do
        echo -e "\033[32mcurl -X $method $url -v\033[0m"
        curl -X $method $url -v
        echo
        sleep 1 # サーバーに負荷をかけないように間隔を設ける
    done
done
