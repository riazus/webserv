#!/usr/bin/python3

import cgi, os

form = cgi.FieldStorage()

# Get filename here
fileitem = form['userfile']

# Test if the file was uploaded
if fileitem.filename:
   open(os.getcwd() + '/cgi-bin/tmp/' + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
   message = 'The file "' + os.path.basename(fileitem.filename) + '" was uploaded to ' + os.getcwd() + '/cgi-bin/tmp'
else:
   message = 'Uploading Failed'

print('<'"!DOCTYPE html"'>')
print('<'"html"'>')
print("<H1> " + message + " </H1>")
print('<'"html"'>')