# !/usr/bin/env python3

from datetime import datetime

now = datetime.now()

current_time = now.strftime("%H:%M:%S")


print('<'"!DOCTYPE html"'>')
print('<'"html"'>')
print('<'"head"'>')
print('\t''<'"link rel="'"'"stylesheet"'"'" href=" '"' "time/style.css" '"''>')
print('\t''<'"link rel="'"'"stylesheet"'"'" href=" '"' "style.css" '"''>')
print('<'"/head"'>')
print('<'"title"'>'"Current time"'<'"/title"'>')
print('\t''<'"body bgcolor="'"'"#c0c0c0"'"''>')
print('\t''\t''<'"div"'>')
print('\t''\t''<'"center class="'"'"text"'"''>')
print('\t''\t''<'"br"'>')
print('\t''\t''<'"br"'>')
print('\t''\t''<'"br"'>')
print('\t''\t''<'"br"'>')
print(current_time)
print('\t''\t''<'"/center"'>')
print('\t''\t''<'"/div"'>')
print('\t''<'"/body"'>')
print('<'"html"'>')