#!/usr/bin/env python3
import os
import cgi

print("Content-Type: text/html")
print()
print("<html><head><title>CGI Test</title></head><body>")
print("<h1>Hello from CGI!</h1>")

# Afficher les variables d'environnement
print("<h2>Environment Variables</h2>")
print("<pre>")
for key, value in os.environ.items():
    print(f"{key}: {value}")
print("</pre>")

# Afficher les paramètres CGI
form = cgi.FieldStorage()
print("<h2>CGI Parameters</h2>")
print("<pre>")
for key in form.keys():
    print(f"{key}: {form[key].value}")
print("</pre>")

print("</body></html>")