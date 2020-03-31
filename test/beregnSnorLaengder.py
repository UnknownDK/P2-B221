import numpy as np #numpy er et matematik bibliotek

#Grundplanets hjørnepunkter defineres
A = np.array([0,0,0])
B = np.array([200,0,0])
C = np.array([0,200,0])
D = np.array([200,200,0])

#Probepunktets koordinater indtastes af brugeren
x = int(input("x-koordinat: "))
y = int(input("y-koordinat: "))
z = int(input("z-koordinat: "))

#Probepunkt defineres
Q = np.array([x,y,z])

#snorvektorerne findes
aq = Q - A
bq = Q - B
cq = Q - C
dq = Q - D

#Længderne af snorvektorerne findes
aqLen = np.sqrt(aq[0]**2 + aq[1]**2 + aq[2]**2)
bqLen = np.sqrt(bq[0]**2 + bq[1]**2 + bq[2]**2)
cqLen = np.sqrt(cq[0]**2 + cq[1]**2 + cq[2]**2)
dqLen = np.sqrt(dq[0]**2 + dq[1]**2 + dq[2]**2)

#Længderne af snorvektorerne afrundes og printes
print("aq: {}, bq: {}, cq: {}, dq: {}".format(round(aqLen, 2), round(bqLen, 2), round(cqLen, 2), round(dqLen, 2)))
