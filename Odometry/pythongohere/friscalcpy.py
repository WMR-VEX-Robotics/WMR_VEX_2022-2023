from lib2to3.pgen2.token import NEWLINE
import numbers as numpy
import math
import time


seconds = 10
contSeconds = 0.25
woFrisKG = 0.065
gPull = 9.8
diaMeter = 140.00
radIus = 70.00
thickNess = 20.00
Ffric1 = 1.0
angVelo = 3200

Velod = radIus * angVelo
acD = Velod * contSeconds
wVelod = woFrisKG * acD
nwVelod = wVelod - (Ffric1 * seconds)
accelerated = nwVelod / woFrisKG
speed = accelerated / contSeconds


print(nwVelod) 
print("N")
print(accelerated)
print("m/s")
