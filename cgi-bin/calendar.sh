#!/bin/bash

echo '<!DOCTYPE html>'
echo '<html>'

# echo '<head>'
# echo '</head>'
echo '<body bgcolor="#c0c0c0"'
echo '<center>'

echo '<br>'


echo 'Calendar:'


echo ''
echo '<pre>' 
cal 
echo '</pre>' 
echo ''

echo '<h3>'
echo 'Date: '
echo '</h3>'
echo '<pre>'
date
date -u
echo '</pre>'

echo '<h4>'
echo 'Days have passed since the beginning of the year: '
echo '</h4>'
echo '<pre>'
date +%j
echo '</center>'
echo '</pre>'

echo '</body>'
echo '</html>'

