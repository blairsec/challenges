from PIL import Image

im = Image.open("output.png")
im2 = Image.open("breathe.jpg")

width, height = im.size

def decode(i, compare):
    i = list(str(i).zfill(len(str(compare))))
    return i[0]


s = ""

for j in range(height):
    for i in range(width):
        data = []
        
        for a, compare in zip(im.getpixel((i,j)), im2.getpixel((i, j))):
            data.append(decode(a, compare))

        s += ''.join(data)



s = list(s)

data = []

while len(s) > 0:
    t = ""
    curr = s.pop(0)

    if curr != "1":
        t += curr + s.pop(0)

    else:
        t += curr + s.pop(0) + s.pop(0)

    data.append(t)

data = ''.join([chr(int(i)) for i in data])


import re

r1 = re.findall(r"actf{.*?}", data)

min = min(map(len, r1))

for i in r1:
	if len(i) == min:
		print(i)
