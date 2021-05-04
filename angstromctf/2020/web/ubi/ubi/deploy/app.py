from flask import Flask, request, jsonify

from Crypto.Cipher import PKCS1_OAEP
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA

import subprocess
import hashlib
import os
import json
import random

allowed_headers = ('content-type', 'content-disposition', 'x-ubi-src')
def verify_headers(headers, signature, key, build):
	if set(headers) - set(allowed_headers): return None
	key = RSA.import_key(key)
	headers = '\n'.join(header+': '+headers[header] for header in sorted(headers.keys()))+f"\nx-ubi-id: {build}\nx-ubi-key: {SHA256.new(key.export_key('PEM', pkcs=8)+bytes([10])).hexdigest()}"
	h = SHA256.new(headers.encode('utf-8')).digest()
	if PKCS1_OAEP.new(key).decrypt(bytes.fromhex(signature)) != h: return None
	return {h.split(': ')[0]: h.split(': ')[1] for h in headers.split("\n")}

app = Flask(__name__)

@app.route('/build', methods=['POST'])
def build():
	if len(request.form['src']) > 500000: return jsonify({'status': 'error'})
	i = hex(random.randrange(2**64))[2:].zfill(16)
	os.mkdir('build/'+i)
	f = open('build/'+i+'/config.json', 'w')
	json.dump({'referer': request.form.get('referer'), 'key': request.form.get('key')}, f)
	f = open('build/'+i+'/source.c', 'w')
	f.write(request.form['src'])
	f.close()
	p = subprocess.run(['/usr/local/bin/timeout', '--no-info-on-success', '-m', '100000', '-t', '1', 'gcc', '-o', '/dev/stdout', 'build/'+i+'/source.c'], capture_output=True)
	if len(p.stderr) or len(p.stdout) > 500000: return jsonify({'status': 'error', 'message': p.stderr.decode('utf-8')}), 400
	f = open('build/'+i+'/a.out', 'wb')
	f.write(p.stdout)
	f.close()
	return jsonify({'status': 'success', 'id': i})

@app.route('/<i>/<name>')
def executable(i, name):
	try:
		sig = '' 
		headers = {}
		for arg in request.args:
			if arg == 'sig':
				sig = request.args[arg]
				continue
			headers[arg] = request.args[arg]
		config = json.load(open('build/'+i+'/config.json'))
		if request.headers.get('referer') != config['referer']: return jsonify({'status': 'error', 'message': 'invalid referer'}), 400
		headers = verify_headers(headers, sig, config['key'], i)
		if not headers: return jsonify({'status': 'error', 'message': 'invalid signature'}), 400
		return open('build/'+i+'/a.out', 'rb').read() if not headers.get('x-ubi-src') else open('build/'+i+'/source.c').read(), headers
	except:
		return jsonify({'status': 'error'}), 400

@app.route('/')
def index():
	return f"""<pre>
Welcome to the Universal Build Integrator!

__/\\\\\\________/\\\\\\__/\\\\\\\\\\\\\\\\\\\\\\\\\\____/\\\\\\\\\\\\\\\\\\\\\\_        
 _\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\/////////\\\\\\_\\/////\\\\\\///__       
  _\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\_______\\/\\\\\\_____\\/\\\\\\_____      
   _\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\______\\/\\\\\\_____     
    _\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\/////////\\\\\\_____\\/\\\\\\_____    
     _\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\_______\\/\\\\\\_____\\/\\\\\\_____   
      _\\//\\\\\\______/\\\\\\__\\/\\\\\\_______\\/\\\\\\_____\\/\\\\\\_____  
       __\\///\\\\\\\\\\\\\\\\\\/___\\/\\\\\\\\\\\\\\\\\\\\\\\\\\/___/\\\\\\\\\\\\\\\\\\\\\\_ 
        ____\\/////////_____\\/////////////____\\///////////__

Never again suffer at the hands of the bourgeoisie. The UBI brings the power of building to the people.

Source:

{open(__file__).read().replace("<", "&lt;").replace(">", "&gt;")}
</pre>"""

if __name__ == "__main__":
	app.run('0.0.0.0', 5000)
