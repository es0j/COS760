
#ifdef BURMA14
#include "burma14.h"
#else
#include "ulysses22.h"
#endif


#ifdef MPI
#include <mpi.h>
#else
#include <omp.h>
#endif 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BRUTEFORCE_SIZE  TOTAL_CITIES - JOB_SIZE

double getTime(){

#ifdef MPI
    return MPI_Wtime();
#else
    return omp_get_wtime();
#endif 
}

//Some utils functions
int inArray(char val,char *arr,int size){
    for(int i=0;i<size;i++){
        if(arr[i]==val){
            return 1;
        }
    }
    return 0;
}

int printArray(char *arr,int size){
    for(int j=0;j<size; j++){
        printf("%d ",arr[j]);
    }
    printf("\n");
}

float computeLen(char *path,int size){

    float total=0;
    for(int i=0;i<size;i++){
        total+=distanceMatrix[path[i]][path[(i+1)%size]];
    }
    return total;

}

void copy(char *dst, char*src,int size){
    for(int i=0;i<size;i++){
        dst[i]=src[i];
    }
}

//https://www.geeksforgeeks.org/c-program-to-print-all-permutations-of-a-given-string/
void swap(char *x, char *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

struct bestPath{
    float cost;
    char path[TOTAL_CITIES];
};

/* Function to print permutations
   of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the string.
*/
void permute(char *a, int l, int r,struct bestPath *output)
{
    int i;
    if (l == r) {
        float res = computeLen(a,BRUTEFORCE_SIZE);
        if( res < output->cost )
        {
            output->cost = res;
            memcpy(output->path,a,sizeof(output->path));
            //printf("New best %f\n",res,output);
            //printArray(a,BRUTEFORCE_SIZE);

        }

    }

    else
    {
        for (i = l; i <= r; i++)
        {
            swap((a + l), (a + i));
            permute(a, l + 1, r,output);
            //backtrack
            swap((a + l), (a + i));
        }
    }
}

//Serial unit of work
void solve(char path[JOB_SIZE], struct bestPath *result){

    double start,end;
    float startCost;
    char pathes[BRUTEFORCE_SIZE];
    char complete_path[TOTAL_CITIES];
    float total_cost;
    int c=0;

    start = getTime();

    for (int i=0;i<TOTAL_CITIES;i++){
        //if not visited
        if(!inArray(i,path,JOB_SIZE)){
            //add to bruteforce
            pathes[c]=i;
            c++;
        }
    }

    result->cost=999999999999999;
    startCost = computeLen(path,JOB_SIZE);

    permute(pathes,0,BRUTEFORCE_SIZE-1,result);

    result->cost += startCost;

    copy(complete_path,path,JOB_SIZE);
    copy(&complete_path[JOB_SIZE],result->path,BRUTEFORCE_SIZE);

    copy(result->path,complete_path,TOTAL_CITIES);

    end = getTime();
    //printf("Serial unity work time:%lf\n",end-start);

}

//solve multiple units of work, used only by MPI
void solveJob(int rank,int processes,struct bestPath *best){
    struct bestPath new;

    //printf("solving job rank %i\n",rank);
    best->cost=99999999999;

    int i=rank;
    while( i <  TOTAL_JOBS){
        solve(jobs[i], &new);
        if(new.cost < best->cost){
            //printf("rank (%i) cost (%f)",rank,new.cost);
            //printArray(new.path,TOTAL_CITIES);
            memcpy(best,&new,sizeof(struct bestPath));
        }
        i +=processes;
    }

    //printf("finished job %i...\n",rank);
    //printf("Total Best path, cost(%f)\n",best->cost);
    //printArray(best->path,TOTAL_CITIES);
}


void main(int argc, char** argv) {

    struct bestPath *best;
    double start,end;
    struct bestPath resultArr[TOTAL_JOBS];
    int finalSearchIndex;

//mpi parallel block
#ifdef MPI
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    start = getTime();

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    finalSearchIndex = world_size;

    MPI_Status Stat;
    if ( rank == 0 ) {
        printf("max rank %d\n",world_size);
        solveJob(rank,world_size,resultArr);
        for(int i=1;i<world_size;i++){
            MPI_Recv(&resultArr[i], sizeof(struct bestPath), MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Stat);
        }

    }
    else{
        solveJob(rank,world_size,&resultArr[rank]);
        MPI_Send(&resultArr[rank], sizeof(struct bestPath), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        MPI_Finalize();
        return;
    }

#else
    start = getTime();
    finalSearchIndex = TOTAL_JOBS;
    printf("Numero max de threads= %d\n", omp_get_max_threads());
    #pragma omp parallel for  shared(resultArr)
    for (int i = 0; i < TOTAL_JOBS; i++)
    {
      solve(jobs[i], &resultArr[i]);
    }
#endif

//common serial block


    best = &resultArr[0];
    for(int i=0;i<finalSearchIndex;i++){
        if(resultArr[i].cost < best->cost){
            best = &resultArr[i];
        }
    }
    printf("Best cost: %lf\n",best->cost);
    printArray(best->path,TOTAL_CITIES);

    end = getTime();

    printf("elapsed time %lf \n",end-start);


#ifdef MPI
    // Finalize the MPI environment.
    MPI_Finalize();
#endif



}