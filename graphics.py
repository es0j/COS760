openmp=[
    (1,379.84,6*60+18.85),
    (2,189.25,3*60+9.25),
    (4,94.76,1*60+34.76),
    (8,47.68,47.68),
    (16,23.77,23.77),
    (32,16.53,16.53),
    (48,13.99,13.99),
    (64,14.29,14.30),
    (128,13.94,13.95),
    (256,13.94,13.95),
    (512,13.85,13.85),
        ]
mpi=[
    (1,388.90,6*60+29.40),
    (2,194.51,3*60+15.09),
    (4,97.27,1*60+37.90),
    (8,48.63,49.24),
    (16,24.36,25.062),
    (20,19.63,20.35),
    (22,17.86,18.62),
    (24,16.26,17.02),
    (26,27.48,28.38),
    (32,22.52,23.48),
    (48,15.33,16.46),
    (64,22.46,24.20),
    (128,17.96,20.82),
    (256,16.94,24.73),
    (512,19.43,32.47),
        ]

#24 , 22.9
def computespeedup(data,real=0):
    processors=[]
    speedup=[]
    for d in data:
        processors.append(d[0])
        speedup.append(data[0][1+real]/d[1+real])
    return processors, speedup

processors,mpi_m = computespeedup(mpi)
_,mpi_r = computespeedup(mpi,1)

#_,omp_m = computespeedup(openmp)
#_,omp_r = computespeedup(openmp,1)

import numpy as np
import matplotlib.pyplot as plt


fig = plt.figure()


plt.plot(processors, processors, '--', label='linear')

plt.plot(processors, mpi_m,  label='mpi_measured')
plt.plot(processors, mpi_r,  label='mpi_real')

#plt.plot(processors, omp_m,  label='OpenMP_measured')
#plt.plot(processors, omp_r,  label='OpenMP_real')

#plt.errorbar(x, y + 2, yerr=yerr, uplims=True, label='uplims=True')

#plt.errorbar(x, y + 1, yerr=yerr, uplims=True, lolims=True,
#             label='uplims=True, lolims=True')

#upperlimits = [True, False] * 5
#lowerlimits = [False, True] * 5
#plt.errorbar(x, y, yerr=yerr, uplims=upperlimits, lolims=lowerlimits,
#             label='subsets of uplims and lolims')

plt.legend(loc='lower right')
plt.show()