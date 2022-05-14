#!/usr/local/bin/python3

import io, qrcode, string

flag_contents = "not_quadres"
assert all(i in string.ascii_lowercase + '_' for i in flag_contents)

flag = b"actf{" + flag_contents.encode() + b"}"
print("flag is %d characters" % len(flag))

qr = qrcode.QRCode(version=1, error_correction = qrcode.constants.ERROR_CORRECT_L, box_size=1, border=0)
while 1:
	try:
		inp = bytes.fromhex(input("give input (in hex): "))
		assert len(inp) == len(flag)
	except:
		print("bad input, exiting")
		break
	qr.clear()
	qr.add_data(bytes([i^j for i,j in zip(inp, flag)]))
	f = io.StringIO()
	qr.print_ascii(out=f)
	f.seek(0)
	print('\n'.join(i[:11] for i in f))
