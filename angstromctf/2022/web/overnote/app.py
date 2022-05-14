from flask import Flask, render_template, request
from flask_socketio import SocketIO
from hashlib import sha256
import os

app = Flask(__name__)

socketio = SocketIO(app)


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/login", methods=["POST"])
def login():
    username = request.form["username"]
    password = request.form["password"]

    session = sha256(password.encode("utf-8")).hexdigest()

    try:
        with open(f"users/{username}", "r") as f:
            content = f.read().split("\n")
            if content[0] == password:
                return render_template(
                    "account.html",
                    username=username,
                    session=session,
                    content="\n".join(content[1:]),
                )
            else:
                return render_template("index.html", error="Wrong username or password")
    except FileNotFoundError:
        return render_template("index.html", error="Wrong username or password")


@app.route("/register", methods=["POST"])
def register():
    username = request.form["username"]
    password = request.form["password"]

    if password == "" or "\n" in password:
        return

    with open(f"users/{username}", "w") as f:
        f.write(password)

    return render_template("index.html")


@socketio.on("update")
def update_paste(data):
    new_content = data["new_content"]
    username = data["username"]
    session = data["sess"]
    content = []

    try:
        with open(f"users/{username}", "r") as f:
            content = f.read().split("\n")
    except:
        return

    if sha256(content[0].encode("utf-8")).hexdigest() == session:
        os.system(f'./perf/perf "users/{username}" "{content[0]}" "{new_content}"')
    else:
        return


if __name__ == "__main__":
    socketio.run(app, debug=True)
