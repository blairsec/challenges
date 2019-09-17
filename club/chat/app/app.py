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
	ws.send(json.dumps({"message": "Welcome!", "type": "message"}))
	for u in chats:
		if u[1].closed:
			chats.remove(u)
	if not session["user"]: return
	chat = [session["user"], ws, None, None, []]
	chats.append(chat)
	while not ws.closed:
		try:
			m = json.loads(ws.receive())
			if m["type"] == "invite":
				sent = False
				for u in chats:
					if u[0] == m["username"]:
						u[1].send(json.dumps({"type": "invite", "username": session["user"], "id": id(ws)}))
						u[4].append(id(ws))
						sent = True
				if not sent:
					chat[1].send(json.dumps({"type": "error", "message": "Error: User is not online."}))
				else:
					chat[1].send(json.dumps({"type": "status", "message": "invite_sent"}))
			elif m["type"] == "accept":
				for u in chats:
					if id(u[1]) == m["id"]:
						u[2] = session["user"]
						u[3] = ws
						chat[2] = u[0]
						chat[3] = u[1]
						u[1].send(json.dumps({"type": "status", "message": "invite_accepted"}))
						u[1].send(json.dumps({"type": "message", "message": "Now chatting with "+u[2]+"."}))
						u[3].send(json.dumps({"type": "message", "message": "Now chatting with "+u[0]+"."}))
						break
				else:
					chat[1].send(json.dumps({"type": "status", "message": "User is not online."}))
			elif m["type"] == "message":
				chat[1].send(json.dumps({"type": "message", "message": session["user"]+": "+m["message"]}))
				chat[3].send(json.dumps({"type": "message", "message": session["user"]+": "+m["message"]}))
		except Exception as e:
			print(e)
	for u in chats:
		for i in u[4]:
			if i == id(ws):
				u[1].send(json.dumps({"type": "status", "message": "invite_removed", "id": i}))
				u[4].remove(i)
		if u[3] == ws:
			u[1].send(json.dumps({"type": "message", "message": session["user"] + " left the chat."}))
	chats.remove(chat)

@app.route('/')
def index():
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
