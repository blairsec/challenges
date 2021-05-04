from PIL import Image

with Image.open("orig.png") as i:
	p = i.load()
	for w in range(i.width):
		for h in range(i.height):
			p[w, h] = (p[w, h][0], p[w, h][1], p[w, h][2], 0)
	i.save('fish.png')
