#!/usr/bin/env python3

import cgi
import sys
import os

# POSTリクエストかどうかを確認
if os.environ["REQUEST_METHOD"] == "POST":
    # POSTデータを取得
    form = cgi.FieldStorage()

    # フォームデータを取得
    title = form.getvalue("title", "")
    content = form.getvalue("content", "")

    # TODO: 入力バリデーションを行う

    # TODO: データベースに記事を保存または更新

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
