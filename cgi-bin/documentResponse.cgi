#!/usr/bin/env python3
import cgi

print("Status: 200 OK")
print("Content-Type: text/html\n")
print("<html><head><title>Simple CGI Script</title></head><body>")
print("<h1>Hello, CGI World!</h1>")

form = cgi.FieldStorage()
print("<h2>Hello, {}!</h2>".format(form.getvalue("nickname")))


print("</body></html>")
