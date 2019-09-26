import random
import requests
import string

from flask import Flask
from flask import request, session
from flask import make_response, redirect, render_template
from flask import flash

app = Flask(__name__)
app.secret_key = 'secret_key'

db = dict()
db['admin'] = {
	'password': 'defundtj',
	'litecoin': float('inf'),
	'ether': float('inf'),
	'bitcoin': float('inf'),
}
db['defund'] = {
	'password': 'defund',
	'litecoin': 0,
	'ether': 0,
	'bitcoin': 0,
}

def csrftoken(username, refresh=False):
	generate = lambda: ''.join(random.choices(string.ascii_letters, k=64))
	if not db[username].get('csrftoken'):
		db[username]['csrftoken'] = generate()
	csrftoken = db[username]['csrftoken']
	if refresh:
		db[username]['csrftoken'] = generate()
	return csrftoken

@app.route('/', methods=['GET'])
def home():
	if session.get('username') in db:
		username = session['username']
		resp = make_response(render_template('home.html',
			username=username,
			litecoin=db[username]['litecoin'],
			ether=db[username]['ether'],
			bitcoin=db[username]['bitcoin'],
			csrftoken=csrftoken(username),
		))
		resp.headers['Access-Control-Allow-Origin'] = '*'
		return resp
	return render_template('auth.html')

@app.route('/register', methods=['POST'])
def register():
	username = request.form.get('username')
	password = request.form.get('password')
	if not db.get(username):
		db[username] = {
			'password': password,
			'litecoin': 0,
			'ether': 0,
			'bitcoin': 0,
		}
		session['username'] = username
	else:
		flash('Username already taken.', 'error')
	return redirect('/')

@app.route('/login', methods=['POST'])
def login():
	username = request.form.get('username')
	password = request.form.get('password')
	if db.get(username):
		if db[username]['password'] == password:
			session['username'] = username
		else:
			flash('Invalid credentials.', 'error')
	else:
		flash('Account does not exist.', 'error')
	return redirect('/')

@app.route('/logout', methods=['GET'])
def logout():
	if session.get('username') in db:
		del session['username']
	return redirect('/')

def transfer(currency, sender, recipient, amount):
	if amount <= db[sender][currency] and amount > 0:
		db[sender][currency] -= amount
		db[recipient][currency] += amount
		flash('Transaction completed.', 'success')
	else:
		flash('Insufficient funds.', 'error')

@app.route('/litecoin', methods=['GET'])
def litecoin():
	print(request.headers)
	recipient = request.args.get('recipient')
	amount = request.args.get('amount')
	if not db.get(recipient):
		print('rip')
		flash('Username does not exist.', 'error')
		return redirect('/')
	try:
		amount = int(amount)
	except:
		flash('Amount must be a positive integer.', 'error')
		return redirect('/')
	sender = session.get('username')
	print('sender: {}'.format(sender))
	if sender in db:
		transfer('litecoin', sender, recipient, amount)
	return redirect('/')

@app.route('/ether', methods=['POST'])
def ether():
	recipient = request.form.get('recipient')
	amount = request.form.get('amount')
	if not db.get(recipient):
		flash('Username does not exist.', 'error')
		return redirect('/')
	try:
		amount = int(amount)
	except:
		flash('Amount must be a positive integer.', 'error')
		return redirect('/')
	sender = session.get('username')
	if sender in db:
		transfer('ether', sender, recipient, amount)
	return redirect('/')

@app.route('/bitcoin', methods=['POST'])
def bitcoin():
	recipient = request.form.get('recipient')
	amount = request.form.get('amount')
	if not db.get(recipient):
		flash('Username does not exist.', 'error')
		return redirect('/')
	try:
		amount = int(amount)
	except:
		flash('Amount must be a positive integer.', 'error')
		return redirect('/')
	sender = session.get('username')
	if sender in db:
		if request.form.get('csrftoken') == csrftoken(sender, refresh=True):
			transfer('bitcoin', sender, recipient, amount)
		else:
			flash('Invalid CSRF token.', 'error')
	return redirect('/')

if __name__ == '__main__':
	app.run()