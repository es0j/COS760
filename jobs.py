#python3 jobs.py > jobs.h
from itertools import permutations
"""
cities = [
    
       (16.47,96.10),
       (16.47,94.44),
       (20.09,92.54),
       (22.39,93.37),
       (25.23,97.24),
       (22.00,96.05),
      (20.47,97.02),
      (17.20,96.29),
      (16.30,97.38),
      (14.05,98.12),
      (16.53,97.38),
      (21.52,95.59),
      (19.41,97.13),
      (20.09,94.55)
  
]
"""
FNAME="burma14.tsp"

cities=[]
with open(FNAME) as f:
    for l in f:
        if l[0]==" " and not "EOF" in l:
            d = l.replace("\n","").split(" ")
            x = float(d[2])
            y = float(d[3])
            cities.append((x,y))
#print(cities)
   

    
    

TOTAL_CITIES = len(cities)

INITIAL_PERMUTE = 4

jobs = list(permutations(range(1,TOTAL_CITIES-1), INITIAL_PERMUTE))


def printJobs():
    print(f"#define TOTAL_JOBS {len(jobs)}")
    print(f"#define TOTAL_CITIES {TOTAL_CITIES}")
    print(f"#define JOB_SIZE {INITIAL_PERMUTE+1}")

    print(f"int jobs[{len(jobs)}][{INITIAL_PERMUTE+1}]="+"{")
    for j in jobs:
        print("{0,",end="")
        for i in range(len(j)):
            print(f"{j[i]},",end="")
            #if(i<INITIAL_PERMUTE-1):
            #    print(",",end="")
        print("},")
    print("};")


def getDistances(i,j):
    return ((cities[i][0]-cities[j][0])**2 + (cities[i][1]-cities[j][1])**2)**0.5

def printDistances():
    print(f"float distanceMatrix[TOTAL_CITIES][TOTAL_CITIES] ="+"{")
    for i in range(TOTAL_CITIES):
        print("{",end="")
        for j in range(TOTAL_CITIES):
            print(getDistances(i,j),end=",")
        print("},")
    print("};")

printJobs();  
printDistances()