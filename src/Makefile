EXECS=tspMPI tspOMP
MPICC?=mpicc

all: ${EXECS}

tspMPI: tsp.c
	${MPICC} -o tspMPI -std=c99 tsp.c -DMPI -DBURMA14

tspOMP: tsp.c
	gcc -fopenmp -std=c99 tsp.c -o tspOMP -DBURMA14

tspMPI22: tsp.c
	${MPICC} -o tspMPI22 -std=c99 tsp.c -DMPI 
tspOMP22: tsp.c
	gcc -fopenmp -std=c99 tsp.c -o tspOMP22
clean:
	rm ${EXECS}

#OMP_NUM_THREADS=4 ./tspOMP