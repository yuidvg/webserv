#!/usr/bin/env python3

print("Status: 200 OK")
print("Content-Type: text/html")
print()

print("""
<html>
<head>
    <title>Redirecting...</title>
    <meta http-equiv="refresh" content="5;url=http://42tokyo.jp/" />
</head>
<body>
    <p>You are being redirected to <a href="http://42tokyo.jp/">www.example.com</a>. Please click the link if you are not redirected within a few seconds.</p>
</body>
</html>
""")
