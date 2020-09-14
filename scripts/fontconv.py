# Convert bitmap font to array. Usage:
#
# python fontconv.py font.png startcharnum onlycaps
#
# author: Miloslav Ciz
# license: CC0 1.0

import sys
from PIL import Image

if len(sys.argv) < 2:
  print("Usage: "+sys.argv[0]+" font.png [start charcode] [caps only] [monospace width]")
  exit(0)

image = Image.open(sys.argv[1])
pixels = image.load()

#count = 0

startChar = ord(' ') if len(sys.argv) < 3 else int(sys.argv[2])

numChars = 128 - startChar

onlyCaps = 0 if len(sys.argv) < 4 or int(sys.argv[3]) == 0 else 1

if onlyCaps:
  numChars -= 128 - ord('^')

monospacewidth = 0 if len(sys.argv) < 5 else int(sys.argv[4])

charW = image.size[0] // numChars
charH = image.size[1]

charHRound = (((charH - 1) // 8) + 1) * 8

charMap = "                                 !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ "
hexaDec = "0123456789ABCDEF"

print("  " + str(charW) + ", " + str(charH) + ", " + str(startChar) + ", " + str(onlyCaps) + ", // width, height, start char, caps only")

for i in range(numChars):
  currentChar = charMap[startChar + i]

  thisWidth = 0

  l = ""

  for x in range(charW):
    col = [0 for j in range(charHRound)]

    for y in range(charH):

      if pixels[i * charW + x,charH - y - 1][0] == 0:

        col[y - (charH - (charHRound - 8))] = 1

        if x + 1 > thisWidth:
          thisWidth = x + 1

    while True:
      n = "0x"
      n += hexaDec[col[0] * 8 + col[1] * 4 + col[2] * 2 + col[3]]
      n += hexaDec[col[4] * 8 + col[5] * 4 + col[6] * 2 + col[7]]

      l += n

      if len(col) > 8:
        l += ", "
        col = col[8:]
      else:
        l += ", " if (x < charW - 1 or i < numChars - 1) else "  "
        break

  if 0 < monospacewidth:
    thisWidth = monospacewidth
  elif thisWidth == 0: # space
    thisWidth = 3

  l = "  0x0" + hexaDec[thisWidth] + ", " + l
  l += "      // " + str(startChar + i) + ": '" + currentChar + "'"

  print(l)
