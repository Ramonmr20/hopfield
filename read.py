import numpy as np
import imageio
import sys

image = imageio.imread(sys.argv[1])

alto = len(image[:,0]) #Altura imagen
ancho = len(image[0]) #Anchura imagen

f = open(sys.argv[2], 'w')

for x in range(alto):
    for y in range(ancho):
        r = image[x][y][0]
        g = image[x][y][1]
        b = image[x][y][2]
        
        g = 0.299*r + 0.587*g + 0.114*b
        
        if g < 255/2:
            g = 0
        else:
            g = 1
            
        f.write(str(g))
        f.write(' ')
