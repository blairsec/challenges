from PIL import Image

flag = "its_all_about_the_penty_yums_baby"

img = Image.open("yankovic_orig.bmp")
pixels = img.load()

for i in range(0,len(flag)):
    for j in range(0,8):
        pixel = list(pixels[0,(i*8)+j])
        bit = (ord(flag[i]) & (1 << j)) >> j
        pixel[2] ^= bit
        img.putpixel((0,(i*8)+j), tuple(pixel))
    print(ord(flag[i]))

img.save("yankovic.bmp")
