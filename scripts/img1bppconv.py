
import sys
import os
from PIL import Image

if len(sys.argv) < 2:
  print("Usage: "+sys.argv[0]+" image.png")
  exit(0)

def is_black(pixel):
    return pixel[0] | pixel[1] | pixel[2] == 0

image = Image.open(sys.argv[1])
pixels = image.load()

img_w = image.size[0]
img_h = image.size[1]

label = os.path.basename(sys.argv[1]).split(".")[0]

sys.stdout.write("\n\
#pragma once\n\
\n\
#include <cstdint>\n\
\n\
constexpr std::uint8_t "+label+"_1bpp[] = {\n"
+str(img_w)+", "+str(img_h)+",\n")

data = [0]*(img_h*(img_w+7)//8)

for y in range(img_h):
    for x in range((img_w+7)//8):
        val = 0
        for bit in range(8):
            val <<= 1
            if 8*x+bit < img_w:
                val |= 0 if is_black(pixels[8*x+bit, y]) else 1
        data[img_w*y//8+x] = val
        sys.stdout.write('0x%02x'%val)
        if x != (img_w-1)//8 or y != img_h-1:
            sys.stdout.write(',')
    sys.stdout.write("\n")
sys.stdout.write("};\n")

