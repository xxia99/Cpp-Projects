#include <iostream>
#include <fstream>
#include <cmath>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[]) {
    ofstream output("Lab2Prob2.txt");
    if (argc != 2)
    {
        output<<"Invalid Input"<<endl;
    }
    unsigned long N = atoll(argv[1]);
    double d_x;
    double Fx = 0.;
    d_x = (log(2) / 7) / N;
#pragma omp parallel for reduction (+:Fx)
    for (int i = 1; i <= N; i++)
    {
        Fx += d_x * 14 * exp(7 * (i-0.5) * d_x);
    }
    output << Fx;
//    cout << Fx;
    return 0;
}