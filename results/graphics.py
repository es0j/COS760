
import re

def parseOMP():
    results=[]
    benchmark={}
    with open("performanceOMP") as f:
        for l in f:
            if "max de threads" in l:
                benchmark["rank"]=float(re.findall(r'\d+',l)[-1])
                
            if "elapsed time" in l:
                benchmark["elapsed"]=float(re.findall(r'\d+\.\d+',l)[-1])
                if(len(results)>0):
                    benchmark["speedup"] = results[0]["elapsed"]/benchmark["elapsed"]
                else:
                    benchmark["speedup"] = 1
                results.append(benchmark)
                benchmark={}
    return results
                
def parseMpi():
    results=[]
    benchmark={}
    with open("performanceMPI") as f:
        for l in f:
            if "max rank" in l:
                benchmark["rank"]=float(re.findall(r'\d+',l)[-1])
                
            if "Serial unity work" in l:
                num=float(re.findall(r'\d+\.\d+',l)[-1])
                if "serial" in benchmark:
                    benchmark["serial"].append(num)
                else:
                    benchmark["serial"]=[num]
            if "Communication time" in l:
                num=float(re.findall(r'\d+\.\d+',l)[-1])
                benchmark["commtime"] = num
            if "elapsed time" in l:
                benchmark["elapsed"]=float(re.findall(r'\d+\.\d+',l)[-1])
                benchmark["avgSerial"] = sum(benchmark["serial"])/len(benchmark["serial"])
                benchmark["worstSerial"] = max(benchmark["serial"])
                if(len(results)>0):
                    benchmark["speedup"] = results[0]["elapsed"]/benchmark["elapsed"]
                else:
                    benchmark["speedup"] = 1
                results.append(benchmark)
                benchmark={}
                
    return results

def singleCorePerformance():
    with open("singleCorePerformance") as f:
        t = f.read()
    nums=re.findall(r'\d+\.\d+',t)
    return [float(x) for x in nums]
single_core=singleCorePerformance()
print("single_core average",sum(single_core)/len(single_core))
mpi_results = parseMpi()
omp_results = parseOMP()
#print(mpi_results)
print(omp_results)

#_,omp_m = computespeedup(openmp)
#_,omp_r = computespeedup(openmp,1)

import numpy as np
import matplotlib.pyplot as plt


fig = plt.figure()

processors_mpi = [x["rank"] for x in mpi_results]
speedup_mpi = [x["speedup"] for x in mpi_results]
plt.plot(processors_mpi, processors_mpi, '--', label='linear')

plt.plot(processors_mpi, speedup_mpi,  label='mpi_results')

processors_omp = [x["rank"] for x in omp_results]
speedup_omp = [x["speedup"] for x in omp_results]
plt.plot(processors_omp, speedup_omp,  label='omp_results')

#plt.yscale("log")
plt.legend(loc='lower right')
plt.show()


avgSerial_mpi = [x["avgSerial"] for x in mpi_results]
worst = [x["worstSerial"] for x in mpi_results]
best = [mpi_results[0]["elapsed"]/processors_mpi[i] for i in range(len(mpi_results))]
commtime = [x["commtime"] for x in mpi_results]

plt.plot(processors_mpi, best, '--', label='theoretical limit')

plt.plot(processors_mpi, avgSerial_mpi,  label='average')
plt.plot(processors_mpi, worst,  label='worst')
plt.plot(processors_mpi, commtime,  label='commtime')



#plt.yscale("log")
plt.legend(loc='lower right')
plt.show()


processors_omp = [x["rank"] for x in omp_results]
comm_time = [x["elapsed"] - (0.03223*(11880/x["rank"]))  for x in omp_results]
total = [x["elapsed"] for x in omp_results]
limit = [0.03223*(11880/x["rank"]) for x in omp_results]

plt.plot(processors_omp, limit, '--', label='theoretical limit')

plt.plot(processors_omp, total,  label='total')
plt.plot(processors_omp, comm_time,  label='comm')
plt.legend(loc='lower right')
plt.yscale("log")
plt.show()

