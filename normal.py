from flask import Flask, render_template
import sqlite3
app = Flask(__name__)


@app.route('/')
def hello_world():
    return render_template('index.html')
@app.route('/create')
def create():
    return render_template('create.html')
@app.route('/about')
def about():
    return render_template('about.html')
@app.route('/sites')
def sites():
    argument = request.args.get('argument')
    return render_template(argument+'.html')
