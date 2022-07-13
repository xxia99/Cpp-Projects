/*
Author: Xin Xia
Class: ECE6122
Last Date Modified: 12/10/2021
Description:
Using the std::thread to solve the ant and seeds problem.
 */
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <random>
#include <atomic>
using namespace std;
std::atomic<long> sum_steps(0); // global counter of steps all threads take in total

// for each thread, do the run for target times, which is allocated by the total runs_num
// and the number of threads
void keepRun(int target)
{
    int s_min = 1;
    int s_max = 4;
    int run_num =0;
    uniform_int_distribution<int> uni_dis(s_min, s_max);
    unsigned direction = hash<thread::id>()(this_thread::get_id());
    default_random_engine x(direction);

    while (run_num <= target) // per thread runs target number of iterations
    {
        int x_current = 2; // x=0,1,2,3,4
        int y_current = 2; // y=0,1,2,3,4
        int x_delta; // x_delta and y_delta indicate which direction to move towards
        int y_delta;
        int steps_per_run = 0; // counter for steps taken by the ant during this run
        int isCarrying = 0; // the flag indicates whether the ant is carrying a seed
        int low_state[] = {1, 1, 1, 1, 1}; // initial state of the lower row
        int top_state[] = {0, 0, 0, 0, 0}; // initial state of the top row

        // current run has not finished
        while (top_state[0] * top_state[1] * top_state[2] * top_state[3] *top_state[4] == 0)
        {
            steps_per_run += 1;
            // select the direction the next step the ant will move towards
            do
            {
                int orientation = uni_dis(x);
                x_delta = 0;
                y_delta = 0;
                if (orientation == 1)
                {
                    x_delta += 1;
                }
                else if (orientation == 2)
                {
                    x_delta += -1;
                }
                else if (orientation == 3)
                {
                    y_delta += 1;
                }
                else
                {
                    y_delta += -1;
                }
            }
            while (x_current + x_delta > 4
                   || x_current + x_delta < 0
                   || y_current + y_delta > 4
                   || y_current + y_delta < 0);
            // randomly reselect the next direction unless the ant will move out of the grid

            x_current += x_delta; // update x_current
            y_current += y_delta; // update y_current

            if (y_current * isCarrying == 4 && top_state[x_current]== 0)
            {
                top_state[x_current] = 1;
                isCarrying = 0;
            } // the ant drops the seed
            if (y_current + isCarrying == 0 && low_state[x_current] == 1)
            {
                low_state[x_current] = 0;
                isCarrying = 1;
            } // the ant picks up the seed
        }

        // last run has finished
        run_num += 1;  // update number of runs in this thread
        sum_steps += steps_per_run; // accumulate steps in the last run
    }
}

int main(int argc, char *argv[])
{
    vector<std::thread> thread_vector;
    ofstream output("ProblemOne.txt");
    output.precision(6);
    int thread_num;
    thread_num = thread::hardware_concurrency(); // get number of threads the CPU of the machine supports
    output << "Number of threads created: " << thread_num << '\n' << endl;
//    cout << "Number of threads created: " << thread_num << '\n' << endl;

    // read runs_num from command line, if not declared set the number to 10,000,000
    unsigned long runs_num;
    if (argc > 1)
    {
        runs_num = atoll(argv[1]);
    }
    else
    {
        runs_num = 10000000;
    }
    if (runs_num % thread_num)
    {
        runs_num = int(runs_num/thread_num)*thread_num;
    }

    int num_per_thread = runs_num / thread_num;
    for (int i = 1; i <= thread_num; ++i)
    {
        thread_vector.push_back(std::thread(keepRun, num_per_thread));
    }
    for (auto &item: thread_vector)
    {
        item.join();
    }
    output << "Expected number of steps: " << sum_steps / (float) runs_num << '\n' << endl;
    output << "Total number of runs needed for solution convergence: " << runs_num << endl;
//    cout << "Expected number of steps: " << sum_steps / (float)runs_num << '\n' << endl;
//    cout << "Total number of runs needed for solution convergence: " << runs_num << endl;
    return 0;
}