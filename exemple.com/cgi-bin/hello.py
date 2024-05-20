#!/usr/bin/env python
import os

print("Content-Type: text/html")
print()
print("<html><body>")
print("<h1>Hello, CGI!</h1>")
print("<p>QUERY_STRING: {}</p>".format(os.environ.get("QUERY_STRING", "")))
print("</body></html>")
