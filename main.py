from flask import Flask, request, render_template, jsonify
import os
import json
from datetime import datetime, timedelta
import hashlib

app = Flask(__name__)


@app.route('/')
def home():
    return render_template("index.html")
@app.route('/create')
def create():
    return render_template("create.html")
@app.route('/login')
def login():
    return render_template("login.html")
@app.route('/about')
def about():
    return render_template("about.html")
def generate_daily_password():
    today = datetime.now().date()
    # Create a unique password by hashing the current date
    pswrd = hashlib.sha256(str(today).encode()).hexdigest()
    pswrd2mrw = hashlib.sha256(str(datetime.now().date()+timedelta(days=1)).encode()).hexdigest()
    pswrd2ds = hashlib.sha256(str(datetime.now().date()+timedelta(days=2)).encode()).hexdigest()
    pswrd3ds = hashlib.sha256(str(datetime.now().date()+timedelta(days=3)).encode()).hexdigest()
    pswrd4ds = hashlib.sha256(str(datetime.now().date()+timedelta(days=4)).encode()).hexdigest()
    print("Todays Code: ", pswrd)
    print("Tommorows Code: ", pswrd2mrw)
    print("3 Days Code: ", pswrd2ds)
    print("4 Days Code: ", pswrd3ds)
    print("5 Days Code: ", pswrd4ds)
    return pswrd
@app.route('/verify-password', methods=['POST'])
def verify_password():
    data = request.get_json()
    password = data.get('password')
    daily_password = generate_daily_password()

    if password == daily_password:
        return jsonify({'status': 'success', 'message': 'Password is correct!'}), 200
    else:
        return jsonify({'status': 'error', 'message': 'Invalid password!'}), 400

@app.route('/check_username', methods=['POST'])
def check_username():
    data = request.get_json()
    username = data.get('username')
    if os.path.exists("templates/usernames/"+username):
        return jsonify({'status': 'error', 'message': 'Username already exists'}), 400
    open("templates/usernames/"+username, "w").write("1")
    return jsonify({'status': 'success', 'message': 'Username is good!'}), 200



@app.route('/create-route', methods=['POST'])
def create_route():
    try:
        data = request.get_json()  # Get JSON data from the request
        route_path = data.get('route_path')  # Extract route path
        template_name = data.get('template_name')  # Extract template name
        template_content = data.get('template_content')  # Extract template content
        username = data.get('username')  # Extract template content
        if template_name and template_content:
            if not os.path.exists("templates/sites/"+username+"_"+template_name):
                with open("templates/sites/"+username+"_"+template_name, 'w') as f:
                    f.write(template_content)
            else:
                return jsonify({'status': 'error', 'message': 'File already exists'}), 400

            # Return success response with created route path
            return jsonify({'status': 'success', 'route_path': route_path}), 201

        # Return error if any field is missing
        return jsonify({'status': 'error', 'message': 'Invalid data: Missing route_path, template_name, or template_content'}), 400

    except Exception as e:
        print("Error occurred:", e)  # Debugging print for error
        return jsonify({'status': 'error', 'message': str(e)}), 500


@app.route('/publish', methods=['POST'])
def publish():
    try:
        data = request.get_json()  # Get JSON data from the request
        template = data.get('template')
        content = data.get('content')
        username = data.get('username')
        if template:
            with open("templates/sites/"+template+".html", 'w') as f:
                f.write(content)
            # Return success response with created route path
            return jsonify({'status': 'success'}), 201

        # Return error if any field is missing
        return jsonify({'status': 'error', 'message': 'Invalid data: Missing template'}), 400

    except Exception as e:
        print("Error occurred:", e)  # Debugging print for error
        return jsonify({'status': 'error', 'message': str(e)}), 500


@app.route('/delete1', methods=['POST'])
def delete1():
    try:
        data = request.get_json()  # Get JSON data from the request
        file = data.get('file')
        if file:
            os.remove("templates/sites/"+username+"_"+file+".html")
            return jsonify({'status': 'success'}), 201

        # Return error if any field is missing
        return jsonify({'status': 'error', 'message': 'Invalid data: Missing template'}), 400

    except Exception as e:
        print("Error occurred:", e)  # Debugging print for error
        return jsonify({'status': 'error', 'message': str(e)}), 500



@app.route('/rename', methods=['POST'])
def rename():
    try:
        data = request.get_json()  # Get JSON data from the request
        old_name = data.get('old_name')
        official_name = data.get('official_name')
        new_name = data.get('new_name')
        print("Old Name: ", old_name == "Untitled")
        if old_name == "Untitled":
            old_name = official_name
        if old_name and new_name:
            open("templates/sites/"+username+"_"+new_name+".html", "w").write(open("templates/sites/"+username+"_"+old_name+".html", "r").read())
            os.remove("templates/sites/"+username+"_"+old_name+".html")
            return jsonify({'status': 'success'}), 201


        return jsonify({'status': 'error', 'message': 'Invalid data: Missing template'}), 400

    except Exception as e:
        print("Error occurred:", e)  # Debugging print for error
        return jsonify({'status': 'error', 'message': str(e)}), 500

@app.route('/sites')
def sites():
    argument = request.args.get('page')
    return render_template("sites/"+argument+'.html')

@app.route('/createSite')
def createSite():
    argument = request.args.get('page')
    if os.path.exists("templates/sites/"+argument+".html"):
        content = open("templates/sites/"+argument+".html", "r").read()
        return render_template("site.html", page=argument, text=content)
    else:
        return render_template("site.html", page=argument, text="None")

if __name__ == '__main__':
    app.run(debug=True)
