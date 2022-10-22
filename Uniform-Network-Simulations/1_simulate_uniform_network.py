import sys
import numpy as np

if len(sys.argv) != 2:
    print("Error: number of nodes not specified.")
    exit()

n = int(sys.argv[1])
x = np.random.uniform(size=n)
y = np.random.uniform(size=n)

out = open("uniform_network.txt", "w")
for i, j in zip(x, y):
    out.write(str(i) + " " + str(j) + "\n")
out.close()