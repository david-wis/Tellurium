#!/bin/bash

set -e
ROOT="$(dirname "$0")"

echo "Relocating script to '$ROOT'..."
cd "$ROOT/.."

# if $2 is set, run npm install selenium-webdriver there, otherwise install selenium-webdriver inside ./output 
if [ -z "$2" ]
then
    echo "Installing/updating selenium-webdriver in ./output..."
    npm install selenium-webdriver --prefix ./output
else
    echo "Installing/updating selenium-webdriver in $2..."
    npm install selenium-webdriver --prefix $2
fi

cat "$1" | ./bin/Compiler $1 $2
