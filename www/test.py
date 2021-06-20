#!/usr/bin/python

# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
name = form.getvalue('name')
company = form.getvalue('company')

print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("</head>")
print ("<body>")
print ("<h2>Hello %s from %s</h2>" % (name, company))
print ("</body>")
print ("</html>")