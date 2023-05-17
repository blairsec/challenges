# requires: flask
import sqlite3
import flask
import time
import os

app = flask.Flask(__name__)
FLAG = os.environ["FLAG"]

@app.get("/")
def index():
    record_holders = [
        ("Old Lady", 0),
        ("Madeline", 10),
        ("Badeline", 10.1),
    ]
    string = map(lambda x: f"<li>{x[0]}: {x[1]} seconds</li>", record_holders)
    return "Welcome to Celeste speedrun records!!!<br>\nCurrent record holders (beat them at <current URL>/play for a flag!):<ol>\n" + "\n".join(string) + "</ol>"

@app.get("/play")
def play():
    return rf"""
<form action="/submit" method="POST">
  <input type="text" style="display: none;" value="{time.time()}" name="start" />
  <input type="submit" value="Press when done!" />
</form>
"""

@app.post("/submit")
def submit():
    try:
        if time.time() < float(flask.request.form["start"]):
            return f"you win the flag: {FLAG}"
        else:
            return "you did not beat all the record holders :("
    except ValueError:
        return "you suck."

if __name__ == "__main__":
    app.run("0.0.0.0")
