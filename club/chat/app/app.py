from flask import Flask, request, session, redirect, render_template
from flask_sockets import Sockets
from flask_session import Session
from tinydb import TinyDB, Query
import gevent
import json

app = Flask(__name__)
app.config['SESSION_TYPE'] = 'filesystem'
app.config['SECRET_KEY'] = 'super secret key'
sockets = Sockets(app)
Session(app)
db = TinyDB('users.json')
User = Query()

chats = []

@sockets.route('/ws')
def socket(ws):
	for u in chats:
		if u[1].closed:
			chats.remove(u)
	if not session["user"]: return
	chats.append((session["user"], ws, None, None))
	while not ws.closed:
		try:
			m = json.loads(ws.receive())
			if m["action"] == "invite":
				
		except Exception as e:
			print(e)

@app.route('/')
def index():
	print(chats)
	if session.get('user'):
		return render_template("home.html")
	else: return redirect("/login")

@app.route('/register', methods=['GET', 'POST'])
def register():
	if request.method == 'POST':
		if request.form.get("username") and request.form.get("password") and request.form.get("ssn"):
			if len(db.search(User.username == request.form.get("username"))) != 0: return "Username already in use."
			db.insert({'username': request.form.get("username"), 'password': request.form.get("password"), 'ssn': request.form.get("ssn")})
			return redirect("/login")
		else:
			return 'Please fill out all required fields.'
	return render_template("register.html")

@app.route('/login', methods=['GET', 'POST'])
def login():
	if request.method == 'POST':
		if db.search((User.username == request.form.get("username")) & (User.password == request.form.get("password"))):
			session['user'] = request.form.get("username")
			session['ssn'] = db.search(User.username == request.form.get("username"))[0]['ssn']
			return redirect("/")
		else: return "Incorrect username or password."
	return render_template("login.html")

@app.route('/logout')
def logout():
	session.clear()
	return redirect("/")
