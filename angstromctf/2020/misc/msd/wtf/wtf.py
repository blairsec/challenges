from PIL import Image

orig_im = Image.open("breathe.jpg")
im = Image.open("output.png")

width, height = im.size

for j in range(height):
    for i in range(width):
        for pix, orig_pix in zip(im.getpixel((i,j)), orig_im.getpixel((i,j))):
            if pix >= 255:
                continue

            orig_len = len(str(orig_pix))

            if str(pix).zfill(3)[4-orig_len:] != str(orig_pix).zfill(3)[4-orig_len:]:
                print(f"Inconsistency: {orig_pix} -> {pix}")
