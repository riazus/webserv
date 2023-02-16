#!/usr/bin/python3

import cgi, os

form = cgi.FieldStorage()

# Get filename here
fileitem = form.getvalue("filename")

# Test if the file was uploaded
if fileitem.getvalue("filename"):
   open(os.getcwd() + '/cgi-bin/tmp/' + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
   message = 'The file "' + os.path.basename(fileitem.filename) + '" was uploaded to ' + os.getcwd() + '/cgi-bin/tmp'
else:
   message = 'Uploading Failed'

print('<'"!DOCTYPE html"'>')
print('<'"html"'>')
print('<'"head"'>')
print('<'"/head"'>')
print('<'"title"'>'"Current time"'<'"/title"'>')
print('\t''<'"body bgcolor="'"'"#c0c0c0"'"''>')
print('\t''\t''<'"div"'>')
print('\t''\t''<'"center class="'"'"text"'"''>')
print('\t''\t''<'"br"'>')
print('\t''\t''<'"br"'>')
print('\t''\t''<'"br"'>')
print('\t''\t''<'"br"'>')
print(message)
print('\t''\t''<'"/center"'>')
print('\t''\t''<'"/div"'>')
print('\t''<'"/body"'>')
print('<'"html"'>')