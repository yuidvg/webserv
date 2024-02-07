#!/usr/bin/env python3
import cgi

print("Status: 200 OK")
print("Content-Type: text/html\n")
print("<html><head><title>Simple CGI Script</title></head><body>")
print("<h1>Hello, CGI World!</h1>")

form = cgi.FieldStorage()
if "name" in form:
    name = form.getvalue("name")
    print("<h2>Hello, {}!</h2>".format(name))
else:
    print("<h2>Hello, unknown person!</h2>")

print("</body></html>")
