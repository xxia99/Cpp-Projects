/*
Author: Xin Xia
Class: ECE 6122
Last Date Modified: 12/4/2021
Description: This is the header file define class ECE_UAV which has member variables to contain the mass,
(x, y, z) position, (vx, vy, yz) velocities, and (ax, ay, az) accelerations of the UAV. The ECE_UAV class
also contains a member variable of type std::thread.
*/
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <eigen3/Eigen/StdVector>

using namespace std;
using namespace chrono;
using namespace Eigen;

class ECE_UAV
{
public:
    int startDemo();  // Function to play the demo
    void endDemo();  // End the simulation once all of the UAV have come within 10 m of the point and the UAVs have flown along the surface for 60 seconds.
    int onGround();  // Keep the UAVs on the ground
    void takeOff();  // Let the UAVs take off
    int apMiddleTop();  // Check if the UAV has approached
    void approached();  // Has approached
    void setUAVPosition(float *angelZ, float *angeXY, float *uavAccleration);  // Set the locations of UAV on the ground
    ECE_UAV(const float uavX, const float uavY );
    void fly2MiddleTop(const float &angelZ, const float &angelXY, float &uavAccleration);  // The UAV flies towards the point (0, 0, 50) above the ground
    int phrase1End();  // Check if the UAV reaches the point (0, 0, 50) above the ground
    void flyonSphere();  // The UAV flies on the surface of a virtual sphere
    thread uavThread;
    void start();
    void join();
    Vector3f* findUAV();  // The function used to sync locations of UAVs
    // Timer used to record the time after demo starts
    const chrono::high_resolution_clock::time_point timer = chrono::high_resolution_clock::now();
private:
    int demoStart = 1;  // 1 starts the demo, 0 ends the demo
    int stay = 1;  // 1 all the UAVs stay on the ground, 0 takes off
    int check = 0; // 1 the UAV has reached (0,0,50), 0 has not
    Vector3f uavOrigin;  // The initial location of the UAVs
    Vector3f uavLocation;  // The dynamic location of the flying UAVs
    Vector3f velocity;  // The velocity of the UAV
    Vector3f acceleration;  // The acceleration of the UAV
};
