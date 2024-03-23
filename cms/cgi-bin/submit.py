#!/usr/bin/env python3

import cgitb
import sys
import os
from urllib.parse import parse_qs

cgitb.enable()

# POSTリクエストかどうかを確認
if os.environ["REQUEST_METHOD"] == "POST":
    # POSTデータを取得
    form_data = sys.stdin.read()
    form = parse_qs(form_data)

    # フォームデータを取得
    title = form.get("title", [""])[0]
    content = form.get("content", [""])[0]

    # /contentディレクトリにファイルを保存
    os.makedirs("/content", exist_ok=True)
    file_path = os.path.join("/content", f"{title}.txt")
    with open(file_path, "w") as file:
        file.write(content)

    # 成功した場合のリダイレクト
    print("Status: 303 See Other")
    print("Location: http://localhost:4242/article/")
    print()
else:
    # POST以外のメソッドでアクセスされた場合は、エラーを表示
    print("Status: 400 Bad Request")
    print("Content-Type: text/plain")
    print()
    print("400 Bad Request")
