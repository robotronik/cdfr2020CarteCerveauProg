#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

#parameters
goal=10
v=-2
x1=0.3#do not set to 0 or a division by 0 will occur
x2=0.5

#internal variables
v=abs(v)
if goal<0:
    v*=-1

t3=goal/(v*(1-0.5*x1-0.5*x2))
t1=x1*t3
t2=t3*(1-x2)

a1=v/t1
a2=v/(t3-t2)

x1=0.5*a1*(t1**2)
x2=v*(t2-t1)+x1

print((t1,t2,t3))

time=np.linspace(0,t3,100)
x=np.zeros(100)

for i,t in enumerate(time):
    if t<t1:
        x[i]=0.5*a1*(t**2)
    elif t<t2:
        x[i]=v*(t-t1)+x1
    else:
        x[i]=-0.5*a2*(t-t2)**2+v*(t-t2)+x2

plt.plot(time,x)
plt.show()
