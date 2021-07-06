import numpy as np
import matplotlib.pyplot as plt
import math as m
import seaborn as sb
import imageio
import os

print('Leyendo datos')
n = 100
data = np.loadtxt('sistema.txt')
grid = []
t = data[:,0]
filenames = []

print('Creando imagenes')
print(len(t))
for k in range(len(t)):
    grid = []
    for i in range(n):
        aux = []
        for j in range(n):
            aux.append(data[k][j+n*i])
        
        grid.append(aux)
        
    
    filename = f'{k}.png'
    filenames.append(filename)
    
    sb.heatmap(grid,xticklabels=False,yticklabels=False,vmin=0,vmax=1)
    plt.title(k)
    plt.savefig(filename)
        
    plt.close()
    print('paso',k,'/',len(t))

print('Creando gif')
with imageio.get_writer('sistema.gif', mode = 'I') as writer:
    for filename in filenames:
        image = imageio.imread(filename)
        writer.append_data(image)
        os.remove(filename)
