#!/bin/bash

set -e

if [ ! -d venv ]; then
    echo 'virtual environment not found, bootstrapping...'
    virtualenv venv -p python3
    source venv/bin/activate
    pip install --upgrade pip
    pip install -r requirements.txt
fi

venv/bin/python run.py
