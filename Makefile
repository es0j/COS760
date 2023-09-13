EXECS=tsp tspOMP
MPICC?=mpicc

all: ${EXECS}

tsp: tsp.c jobs.h
        ${MPICC} -o tsp -std=c99 tsp.c
tspOMP: tspOMP.c jobs.h
        gcc -fopenmp -std=c99 tspOMP.c -o tspOMP
clean:
        rm ${EXECS}