#include "jobs.h"
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BRUTEFORCE_SIZE  TOTAL_CITIES - JOB_SIZE


int inArray(int val,int *arr,int size){
    for(int i=0;i<size;i++){
        if(arr[i]==val){
            return 1;
        }
    }
    return 0;
}

int printArray(int *arr,int size){
    for(int j=0;j<size; j++){
        printf("%d ",arr[j]);
    }
    printf("\n");
}

float computeLen(int *path,int size){

    float total=0;
    for(int i=0;i<size;i++){
        total+=distanceMatrix[path[i]][path[(i+1)%size]];
    }
    return total;

}

void copy(int *dst, int*src,int size){
    for(int i=0;i<size;i++){
        dst[i]=src[i];
    }
}

//https://www.geeksforgeeks.org/c-program-to-print-all-permutations-of-a-given-string/
void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

struct bestPath{
    float cost;
    int path[TOTAL_CITIES];
};

/* Function to print permutations
   of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the string.
*/
void permute(int *a, int l, int r,struct bestPath *output)
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


void solve(int path[JOB_SIZE], struct bestPath *result){

    float startCost;

    int pathes[BRUTEFORCE_SIZE];
    int complete_path[TOTAL_CITIES];
    float total_cost;
    int c=0;

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
    //printf("start cost: %f\n",startCost);
    //printf("permuting: \n");
    //printArray(pathes,BRUTEFORCE_SIZE);
    permute(pathes,0,BRUTEFORCE_SIZE-1,result);

    //printf("Best path, cost(%f)\n",result.cost);
    //printArray(result.path,BRUTEFORCE_SIZE);

    result->cost += startCost;

    copy(complete_path,path,JOB_SIZE);
    copy(&complete_path[JOB_SIZE],result->path,BRUTEFORCE_SIZE);

    copy(result->path,complete_path,TOTAL_CITIES);

    //printf("Total Best path, cost(%f)\n",total_cost);
    //printArray(complete_path,TOTAL_CITIES);
}

int main(int argc, char** argv) {

    struct bestPath *best;
    double start,end;
    struct bestPath resultArr[TOTAL_JOBS];

    start = omp_get_wtime();

    printf("Numero max de threads= %d\n", omp_get_max_threads());
    #pragma omp parallel for  shared(resultArr)
    for (int i = 0; i < TOTAL_JOBS; i++)
    {
      solve(jobs[i], &resultArr[i]);
    }




    best = &resultArr[0];
    for(int i=0;i<TOTAL_JOBS;i++){
        if(resultArr[i].cost < best->cost){
            best = &resultArr[i];
        }
    }
    printf("Best cost: %lf\n",best->cost);
    printArray(best->path,TOTAL_CITIES);

    end = omp_get_wtime();

    printf("elapsed time %lf \n",end-start);


}