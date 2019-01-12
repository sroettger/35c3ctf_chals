#!/usr/bin/env python2
import logging
from flask import Flask, render_template, request, redirect, make_response
from google.appengine.ext import ndb
import string
import random

app = Flask(__name__)

SESSION_COOKIE = 'session'

class User(ndb.Model):
    name = ndb.StringProperty()

class File(ndb.Model):
    owner = ndb.KeyProperty(kind=User)
    name = ndb.StringProperty()
    content = ndb.StringProperty()

def get_user():
    user = None
    try:
        user = ndb.Key('User', request.cookies.get(SESSION_COOKIE)).get()
    except:
        pass
    return user

@app.route('/')
def index():
    user = get_user()
    if not user:
        return redirect("/signup", code=302)

    files = map(lambda f: f.name, File.query(File.owner == user.key).fetch())
    logging.info(files)

    return render_template("index.html", user=user.name, filenames=files)

@app.route('/create', methods=['POST'])
def create():
    if request.headers.get('XSRF') != '1':
        return 'XSRF', 400
    filename = request.form['filename']
    content = request.form['content']
    user = get_user()
    File(owner=user.key, name=filename, content=content).put()
    return ''

@app.route('/signup', methods=['GET', 'POST'])
def signup():
    if get_user():
        return 'Already logged in!', 400

    if request.method == 'POST':
        username = request.form['username']
        if not username:
            return 'Empty username', 400
        if not all(c in string.ascii_lowercase for c in username):
            return 'Only ascii_lowercase allowed', 400
        cookie = ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(20))
        user = User(key=ndb.Key('User', cookie), name=username)
        user.put()
        response = make_response(redirect('/'))
        response.set_cookie(SESSION_COOKIE, cookie)
        return response
    else:
        return render_template('signup.html')

@app.route('/read')
def read_file():
    user = get_user()
    if not user:
        return redirect("/signup", code=302)

    filename = request.args['filename']

    f = File.query(File.owner == user.key, File.name == filename).get()
    if not f:
        return 'Not found', 404

    response = make_response(f.content)
    response.headers['Content-Type'] = 'text/plain'
    response.headers['X-Content-Type-Options'] = 'nosniff'
    return response

@app.route('/search')
def search():
    user = get_user()
    if not user:
        return redirect("/signup", code=302)
    q = request.args.get('q', '')
    files = filter(lambda f: q in f.content, File.query(File.owner == user.key).fetch())
    return render_template("search.html", files=files, q=q)

@app.errorhandler(500)
def server_error(e):
    logging.exception('Error {}'.format(e))
    return 'An internal error occurred.', 500
