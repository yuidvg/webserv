#!/bin/bash

# サーバーのホスト名とポートを指定
HOST="localhost"
PORT=8080
# 同時に実行するリクエストの数
REQUESTS=10

# テスト用のHTTPリクエストを送信する関数
send_request() {
  for (( i=1; i<=REQUESTS; i++ ))
  do
    # curlを使用してHTTPリクエストを送信
    curl -s -o /dev/null "http://$HOST:$PORT" &
  done
}

echo "多重IOテストを開始します。$REQUESTS 回のリクエストを $HOST:$PORT に送信します。"
send_request

# すべてのバックグラウンドプロセスが終了するのを待つ
wait
echo "テスト完了。"
