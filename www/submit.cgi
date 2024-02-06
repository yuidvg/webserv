#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys

def main():
    # 環境変数からPOSTデータの長さを取得
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    # 標準入力からPOSTデータを読み取る
    post_data = sys.stdin.read(content_length)
    
    # ヘッダ出力
    print("Content-Type: text/plain\n")
    
    # POSTデータを出力（デモ用）
    print("Received POST data:")
    print(post_data)

if __name__ == "__main__":
    main()
