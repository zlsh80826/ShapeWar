#!/bin/bash

set -e

cd "$(dirname ${BASH_SOURCE[0]})"

exists () {
    type "$1" &> /dev/null;
}

if [ ! -d venv ]; then
    echo 'virtual environment not found, bootstrapping...'
    if exists virtualenv; then
        virtualenv venv -p python3
    elif exists pyvenv; then
        pyvenv venv
    else
        echo 'Error: virtualenv, pyvenv unavailable'
        exit
    fi

    source venv/bin/activate
    pip install --upgrade pip
    pip install -r requirements.txt
fi

venv/bin/python run.py
