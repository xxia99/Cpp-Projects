/*
Author: <Xin Xia>
Class: ECE6122
Last Date Modified: <11/17/2021>
Description: This is a CUDA program to determine the steady state heat distribution
in a thin metal plate using synchronous iteration on a GPU.
*/
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cuda_runtime.h>
#include "common/inc/helper_cuda.h"
#include "common/inc/helper_functions.h"

using namespace std;

// The function to find the index of input parameters (N or I) in command line.
int findIndex(const int argc, const char **argv, const char *para)
{
    int position;  // The position where the parameter stays.
    for (int i=1; i < argc; i++)
    {
        if (STRNCASECMP(argv[i], para, (int)strlen(para)))
        {
            continue;
        }
        else
        {
            position = i;
            break;
        }
    }
    return position;
}

__global__ void refreshPlate(double *h, int N)
{
    double tempT = 0;
    int pX = blockIdx.x * blockDim.x + threadIdx.x;
    int pY = blockIdx.y * blockDim.y + threadIdx.y;

    // Compute the temperature of the inner points.
    if (pX >= 1 && pX <= (N-2) && pY >= 1 && pY <= (N-2))
        tempT = 0.25 *(h[(pX+1)+pY*N] + h[(pX-1)+pY*N] + h[pX+(pY-1)*N] + h[pX+(pY+1)*N]);
    else
        tempT = h[pX+pY*N];
    h[pX + N*pY] = tempT; // Update the temperatures.
    __syncthreads();
}

void runCalculation(const int &N, const long I)
{
    double *h;  // the array h storing temperature of each point on the plate.
    float execTime = 0;  // the cuda kernel execute time
    unsigned long space = sizeof(double)* N * N;  // the memory should be allocated for the plate
    cudaMallocManaged((void**)&h, sizeof(double) * space);
    dim3 BK(N,1);
    dim3 TD(1,N);
    cudaEvent_t beginEvent;  // when the cuda event starts
    cudaEvent_t endEvent;  // when the cuda event ends

    // Set the initial temperatures on the plate.
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if(i==0 && j<0.7*(N-1) && j>0.3*(N-1))
                h[i*N+j] = 100.0;

            else
                h[i*N+j] = 20.0;
        }
    }

    // Record when the computation begins.
    cudaEventCreate(&beginEvent);
    cudaEventRecord(beginEvent, NULL);

    // Create event when the computation ends.
    cudaEventCreate(&endEvent);

    // Keep refreshing the points on the plate during set iterations.
    for (int i = 0; i < I; ++i)
    {
        refreshPlate<<<BK, TD>>>(h, N);
    }

    // Record and synchronize when the computation ends.
    cudaEventRecord(endEvent, NULL);
    cudaEventSynchronize(endEvent);

    // Calculate the cuda kernel execute time and output it to the console.
    cudaEventElapsedTime(&execTime, beginEvent, endEvent);
    cout << std::setprecision(2) << std::fixed << execTime << endl;
    cudaDeviceSynchronize();

    // Write the temperatures to the finalTemperatures.csv after set the precision of results to 6 decimals.
    fstream out("finalTemperatures.csv", ios::out | ios::trunc);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            out << std::setprecision(6) << std::fixed << h[j+i*N] << ",";
        }
        out << endl;
    }
}


int main(int argc, char **argv)
{
    int N;  // the number of N x N interior points.
    int I;  // the number of iterations.
    int NIndex, IIndex;  // the positions where N and I stay in the command line inputs.
    NIndex = findIndex(argc, (const char **)argv, "-N");
    IIndex = findIndex(argc, (const char **)argv, "-I");

    // -N and -I both should take in a positive integer
    if ( argv[NIndex+1] > 0 && argv[IIndex+1] > 0)
    {
        stringstream(argv[NIndex+1])>>N;
        stringstream(argv[IIndex+1])>>I;
        N += 2;
    }
    else
    {
        cout << "Invalid parameters, please check your values." << endl;
        exit(EXIT_FAILURE);
    }

    // Run the calculation loop for I iterations on the N inner points on the plate.
    runCalculation(N, I);
    return 0;
}