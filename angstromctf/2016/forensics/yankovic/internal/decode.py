from PIL import Image

flag = "its_all_about_the_pentiums_baby"

img_steg = Image.open("yankovic.bmp")
img_orig = Image.open("yankovic_orig.bmp")
pixels_steg = img_steg.load()
pixels_orig = img_orig.load()

for i in range(0,len(flag)):
    byte = 0
    for j in range(7, -1, -1):
        bit = pixels_steg[0,(i*8)+j][2]^pixels_orig[0,(i*8)+j][2]
        byte = (byte << 1) | bit
    print(chr(byte))
