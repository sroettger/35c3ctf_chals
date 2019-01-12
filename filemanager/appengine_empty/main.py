#!/usr/bin/env python2
from flask import Flask

app = Flask(__name__)

@app.route('/')
def index():
    return 'hello world'
